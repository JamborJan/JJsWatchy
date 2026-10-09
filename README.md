# JJ's Watchy Watchface

## Get your dev environment setup

1. Install VSCode
2. Install PlatformIO extension (platformio.platformio-ide)
3. Install C/C++ extension (ms-vscode.cpptools)

## Watchface configuration

To toggle between Dark Mode/Light Mode, change `#define DARKMODE true` in `Watchy_7_SEG.cpp`.

Copy the `secrets.public.h` file and create your own `secrets.private.h` file. This file is already added to `.gitignore` and thus should not get shared. Add your own`OPENWEATHERMAP_APIKEY` and `CITY_ID` to the `.secrets.private.h` file. You can get an API key from [OpenWeatherMap](https://openweathermap.org/api).

## Build and upload

1. Connect your Watchy to your computer
2. Build and upload the code to your Watchy by running the `PlatformIO: Upload` task in VSCode

## Ideas and To-Todos

- [ ] Finalize weather retrieval process: only update when wifi is connected
- [ ] Create a custom menu point for training. Later we can use that for settings or displaying other stuff
- [ ] Add an API request to retreive status of my monitored systems from either Grafana or Portainer or such
