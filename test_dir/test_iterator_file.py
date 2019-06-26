import os
import random

files = os.listdir('./sdk_result/')

print(random.randint(0, len(files)-1))
print(files[0])

print(len(files))

