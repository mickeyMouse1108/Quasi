out = ""
spacing = 16
with open("res/TestBatchedTextured/img2.png","rb") as f:
    out += f.read().hex('|', 1)
    codes = out.split('|')
    out = ',\n    0x'.join((',0x'.join(codes[i:i+16]) for i in range(0, len(codes), 16)))
out = f"{{\n    0x{out}\n}}"
print(out)