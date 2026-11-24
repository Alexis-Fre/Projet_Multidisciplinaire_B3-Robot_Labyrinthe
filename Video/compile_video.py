#!/usr/bin/env python3
"""
Compilateur automatique de la vidéo de démonstration (1080p 30fps, transitions, voix-off, musique de fond).
Utilisation sur Arch Linux :
  1. Décompressez ce pack à la racine du dépôt GitHub (là où se trouvent Photo_projet/ et Video_projet/).
  2. Ajoutez la vidéo du tour complet dans : Video_projet/parcours_labyrinthe.mp4
  3. Placez les enregistrements audio dans ./audios/ nommés :
       bloc1_alexis.m4a (ou .mp3/.wav/.ogg)
       bloc2_papambaye.m4a
       bloc3_alexis.m4a
       bloc4_papambaye.m4a
       bloc5_alexis.m4a
       bloc6_papambaye.m4a
       bloc7_alexis.m4a
       bloc8_papambaye.m4a
  4. Exécutez : python3 compile_video.py
"""
import os, glob, subprocess, tempfile, shutil

BLOCS = [
    ("bloc1_alexis", [
        ("img", "slides/slide_01_intro.png", 0.55),
        ("img", "Photo_projet/Photo_Robot_Monter.jpg", 0.45),
    ]),
    ("bloc2_papambaye", [
        ("vid", "Video_projet/chassis_robot.mp4", 0.45),
        ("vid", "Video_projet/bille_de_direction.mp4", 0.35),
        ("img", "Photo_projet/roue.jpeg", 0.20),
    ]),
    ("bloc3_alexis", [
        ("img", "slides/slide_02_capteurs_135.png", 0.55),
        ("vid", "Video_projet/capteur_distance.mp4", 0.45),
    ]),
    ("bloc4_papambaye", [
        ("vid", "Video_projet/carte_arduino.mp4", 0.30),
        ("vid", "Video_projet/shield.mp4", 0.30),
        ("img", "slides/slide_03_architecture_i2c.png", 0.40),
    ]),
    ("bloc5_alexis", [
        ("vid", "Video_projet/cerveaux_moteurs.mp4", 0.25),
        ("vid", "Video_projet/etalonnage_servos.mp4", 0.35),
        ("vid", "Video_projet/batterie_robot.mp4", 0.20),
        ("vid", "Video_projet/cable_dalimentation.mp4", 0.20),
    ]),
    ("bloc6_papambaye", [
        ("img", "slides/slide_04_fsm_code.png", 1.0),
    ]),
    ("bloc7_alexis", [
        ("img", "slides/slide_05_demo_labyrinthe.png", 0.25),
        ("vid", "Video_projet/parcours_labyrinthe.mp4", 0.75),
    ]),
    ("bloc8_papambaye", [
        ("img", "slides/slide_06_outro.png", 1.0),
    ]),
]

def get_duration(path):
    res = subprocess.run(
        ["ffprobe", "-v", "error", "-show_entries", "format=duration", "-of", "default=noprint_wrappers=1:nokey=1", path],
        capture_output=True, text=True, check=True
    )
    return float(res.stdout.strip())

def find_audio(stem):
    matches = sorted(glob.glob(f"audios/{stem}.*"))
    return matches[0] if matches else None

def main():
    tmp = tempfile.mkdtemp(prefix="robot_vid_")
    try:
        segment_files = []
        for idx, (stem, visuals) in enumerate(BLOCS, 1):
            aud = find_audio(stem)
            if not aud:
                raise FileNotFoundError(f"Audio manquant : audios/{stem}.* (m4a, mp3, wav, ogg)")
            dur = get_duration(aud) + 0.8  # +0.8s de respiration entre blocs
            sub_clips = []
            for v_i, (kind, src, weight) in enumerate(visuals):
                if not os.path.exists(src):
                    raise FileNotFoundError(f"Fichier visuel introuvable : {src}")
                seg_dur = max(1.5, dur * weight)
                sub_out = os.path.join(tmp, f"b{idx}_v{v_i}.mp4")
                vf = "scale=1920:1080:force_original_aspect_ratio=decrease,pad=1920:1080:(ow-iw)/2:(oh-ih)/2:color=#0F172A,fps=30,format=yuv420p"
                if kind == "img":
                    cmd = ["ffmpeg", "-y", "-loop", "1", "-i", src, "-t", f"{seg_dur:.2f}", "-vf", vf, "-c:v", "libx264", "-preset", "fast", "-crf", "20", sub_out]
                else:
                    cmd = ["ffmpeg", "-y", "-stream_loop", "-1", "-i", src, "-t", f"{seg_dur:.2f}", "-an", "-vf", vf, "-c:v", "libx264", "-preset", "fast", "-crf", "20", sub_out]
                subprocess.run(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=True)
                sub_clips.append(sub_out)

            # Concat visuelle du bloc
            list_txt = os.path.join(tmp, f"b{idx}_list.txt")
            with open(list_txt, "w") as lf:
                for sc in sub_clips:
                    lf.write(f"file '{sc}'\n")
            bloc_vid = os.path.join(tmp, f"bloc_{idx}_vid.mp4")
            subprocess.run(["ffmpeg", "-y", "-f", "concat", "-safe", "0", "-i", list_txt, "-c", "copy", bloc_vid], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=True)

            # Mux avec la voix-off normalisée (loudnorm)
            bloc_mux = os.path.join(tmp, f"bloc_{idx}_full.mp4")
            subprocess.run([
                "ffmpeg", "-y", "-i", bloc_vid, "-i", aud,
                "-filter_complex", "[1:a]aresample=44100,loudnorm=I=-16:TP=-1.5:LRA=11,apad=pad_dur=0.8[vo]",
                "-map", "0:v", "-map", "[vo]", "-shortest",
                "-c:v", "copy", "-c:a", "aac", "-b:a", "192k", bloc_mux
            ], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=True)
            segment_files.append(bloc_mux)
            print(f"[OK] Bloc {idx}/8 assemblé ({dur:.1f}s)")

        # Assemblage global + musique de fond à -22 dB
        all_list = os.path.join(tmp, "all_blocs.txt")
        with open(all_list, "w") as af:
            for sf in segment_files:
                af.write(f"file '{sf}'\n")
        merged_no_music = os.path.join(tmp, "merged_no_music.mp4")
        subprocess.run(["ffmpeg", "-y", "-f", "concat", "-safe", "0", "-i", all_list, "-c", "copy", merged_no_music], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=True)

        final_out = "Video_Demonstration_Robot_Labyrinthe_B3.mp4"
        subprocess.run([
            "ffmpeg", "-y", "-i", merged_no_music, "-stream_loop", "-1", "-i", "background_music.wav",
            "-filter_complex", "[1:a]volume=0.28[bg];[0:a][bg]amix=inputs=2:duration=first:dropout_transition=3[outa]",
            "-map", "0:v", "-map", "[outa]", "-c:v", "copy", "-c:a", "aac", "-b:a", "192k", final_out
        ], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=True)
        print(f"\nSuccès ! Vidéo générée : {final_out}")
    finally:
        shutil.rmtree(tmp, ignore_errors=True)

if __name__ == "__main__":
    main()
