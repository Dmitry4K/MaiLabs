import sys
import os
import struct
import pathlib
from PIL import Image
import ctypes
from os import listdir
from os.path import isfile, join

# Список для хранения кадров.
frames = []
def custom_key(file):
    return int(file.rsplit('_')[1].rsplit('.')[0])

def to_gif(path, output_name):

    files = [f for f in listdir(path) if join(path, f).endswith('.jpg')]
    files.sort(key=custom_key)
    for file in files:
        # Открываем изображение каждого кадра.
        frame = Image.open(path+'\\'+file).convert('RGBA')
        # Добавляем кадр в список с кадрами.
        frames.append(frame)
    # Берем первый кадр и в него добавляем оставшееся кадры.
    frames[0].save(
        path+'\\'+output_name,
        save_all=True,
        append_images=frames[1:],  # Срез который игнорирует первый кадр.
        optimize=False,
        duration=40,
        loop=0
    )

def to_img(src, dst=None):
    fin = open(src, 'rb')
    (w, h) = struct.unpack('hi', fin.read(8))
    buff = ctypes.create_string_buffer(4 * w * h)
    fin.readinto(buff)
    fin.close()
    img = Image.new('RGB', (w, h))
    pix = img.load()
    offset = 0
    for j in range(h):
        for i in range(w):
            (r, g, b, _) = struct.unpack_from('cccc', buff, offset)
            pix[i, j] = (ord(r), ord(g), ord(b))
            offset += 4
    if dst:
        img.save(dst)
    else:
        img.show()


def main():
    path = os.path.abspath(os.getcwd())
    print()
    if len(sys.argv) == 2:
        path += sys.argv[1]

    print("RENDERING GIF")
    print(f"PATH OF FOLDER: {path}")
    files = [f for f in listdir(path) if join(path, f).endswith('.data')]
    print(f"TOTAL FILES: {len(files)}")
    for num, file in enumerate(files):
        exported_filename = file.split('.')[0] + '.jpg'
        print("\r                                                                                ", end = '')
        print(f"\rFILE {num} OF {len(files)} RENDERED TO {exported_filename}", end='')
        to_img(path + '\\' + file, path + '\\' + exported_filename)

    #to_gif(os.path.abspath(path), 'res.gif')

if __name__ == '__main__':
    main()
