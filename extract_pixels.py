from PIL import Image
import sys

# Open an image
image_path = sys.argv[1]
image = Image.open(image_path)

# Get pixel data as a 2D list
pixel_data = list(image.getdata())

# Convert the pixel data to a 2D list (no need to reshape for grayscale)
width, height = image.size
pixel_data = [pixel_data[i:i + width] for i in range(0, len(pixel_data), width)]

def getChar(x, y):
    str = "{"
    for row in range(8):
        str += "0b"
        for col in range(5):
            if pixel_data[y + row][x + col] > 127:
                str += "1"
            else:
                str += "0"
        if row != 7:
            str += ","
    str += "}"
    return str

code = "{"
for c in range(256):
    x = (c % 16) * 5
    y = (c // 16) * 8
    code += getChar(x, y)
    if c != 255:
        code += ","
code += "}"

print(code)