import subprocess as sp
import os
import sys
import pandas as pd

multipliers = [1, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1]
triangles = [
    [
        [-1, -1, 0],
        [ 0,  1, 0],
        [ 1, -1, 0]
    ],
    [
        [-1, -1, 0],
        [-1,  1, 0],
        [ 1, -1, 0]
    ],
    [
        [-1.0, -1, 0],
        [ 0.5,  1, 0],
        [ 1,   -1, 0]
    ],
    [
        [ 0, -1, 0],
        [-1,  1, 0],
        [ 1, -1, 0]
    ],
    [
        [-1, -1, 0],
        [ 0,  1, 0],
        [ 1,  0, 0]
    ]
]

powers_of_2 = [128, 64, 32, 16, 8, 4, 2]
needed_frames = 500
output = pd.DataFrame(columns=["triangle_i", "multiplier", "bounding_boxW", "bounding_boxH", "avg_fps", "tileW", "tileH"])

i = 0
for triangle_i, raw_triangle in enumerate(triangles):
    for multiplier in multipliers:
        triangle = [list(map(lambda x: x*multiplier, vertex)) for vertex in raw_triangle]
        triangle_cli_arg = "\;".join([",".join([str(x) for x in vertex]) for vertex in triangle])
        print(f"Triangle index: {triangle_i}; Multiplier: {multiplier}")
        print(f"Resulting triangle: {triangle}")
        for tile_size in powers_of_2:
            args = [str(needed_frames), triangle_cli_arg, f"{tile_size},{tile_size}"]
            print(f"Launching... args: {' '.join(args)}")
            with open("temp.log", "w+") as f:
                sp.Popen(["bin/rasterizer", args[0], args[1], args[2]], stdout=f, stderr=f).wait()
            with open("temp.log", "r") as f:
                fps = []
                bb = False
                for line in f:
                    line_arr = line.split()
                    if line_arr[0] == "I:":
                        fps.append(float(line_arr[5][:-1]))
                    elif bb == False:
                        bb = (line_arr[4][1:] + line_arr[5][:-1]).split(",")
                avg_fps = sum(fps) / len(fps)
                output.loc[i] = [
                    triangle_i, multiplier, bb[0], bb[1], avg_fps, tile_size, tile_size
                ]
                i += 1

os.remove("temp.log")
output.to_csv("output.csv", index=False)

