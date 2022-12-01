line = 0
for x in range(0, 0x100):
    x = (((x & 0xaa) >> 1) | ((x & 0x55) << 1))
    x = (((x & 0xcc) >> 2) | ((x & 0x33) << 2))
    x = (((x & 0xf0) >> 4) | ((x & 0x0f) << 4))
    if line == 0:
        print(f"    0x{x:02x},", end='')
        line = 1
    elif line == 7:
        print(f" 0x{x:02x},")
        line = 0
    else:
        print(f" 0x{x:02x},", end='')
        line += 1
