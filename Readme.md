## WAV MODIFIER

To modify a wav file or raw file.

When you want to change volume,
run:
```
VolumeChange -m -f 0.7 Infile.wav Outfile.wav
// == VolumeChange Infile.wav Outfile.wav -m -f 0.7
```

When you want to convert wav to raw,
run:
```
VolumeChange -r Infile.wav Outfile.raw
// == VolumeChange Infile.wav Outfile.raw -r
```

When you want to convert raw to wav,
run:
```
VolumeChange -w Infile.raw Outfile.wav
// == VolumeChange Infile.raw Outfile.wav -w
```