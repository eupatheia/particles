# particles

Implements particle demos

<img width="600" alt="image" src="https://user-images.githubusercontent.com/75283980/230204121-3c7a005e-e2c2-4089-ad1e-6427d27caebd.png">

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
particles $ mkdir build
particles $ cd build
particles/build $ cmake ..
particles/build $ start particles.sln
```

Your solution file should contain four projects.
To run from the git bash command shell, 

```
particles/build $ ../bin/Debug/billboard-axis.exe
particles/build $ ../bin/Debug/explosion.exe
particles/build $ ../bin/Debug/sparkle-trail.exe
particles/build $ ../bin/Debug/sparkles.exe
```

*macOS*

Open terminal to the directory containing this repository.

```
particles $ mkdir build
particles $ cd build
particles/build $ cmake ..
particles/build $ make
```

To run each program from build, you would type

```
particles/build $ ../bin/billboard-axis
particles/build $ ../bin/sparkles
particles/build $ ../bin/sparkle-trail
particles/build $ ../bin/explosion
```

## Demo of basic features

### Sparkle-Trail

![sparkle-trail](https://user-images.githubusercontent.com/75283980/230204588-9c4e109e-0224-4f6c-b42b-8e811599824f.gif)

### Explosion

![explosion](https://user-images.githubusercontent.com/75283980/230204653-1d73dcab-4012-4908-8ec8-e827621ad42c.gif)

### Axis Billboard

![billboard-axis](https://user-images.githubusercontent.com/75283980/230205241-4e1b04ee-3236-4efc-9213-3e6c1af66ba8.gif)

---

## Unique features 

### Smoke

![smoke](https://user-images.githubusercontent.com/75283980/230205503-cddebbf1-c973-44a6-b882-f5e33af5125a.gif)

### Snow

![snow](https://user-images.githubusercontent.com/75283980/230205643-2215f666-5069-4e87-a984-3bd31d734b1e.gif)
