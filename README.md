# StreamAssistant

This is a program designed to assist streamers that use dynamic data on OBS but don't know / don't want to use html templates.

You can use your own layout images and add your own team logos, map images, team colors and even mode icons to customize your stream. The assistant keeps track of each team's name, logo, score, color and region + maps, modes and winners for a round up to a BO9. You can also add messages, titles, a logo, name and @ for your casters and change it in the spot with this Stream Assistant.

Video tutorial: [https://youtu.be/aRtqBaEw-TA](https://youtu.be/aRtqBaEw-TA)

Getting Started

- Go to the [Releases](https://github.com/guigas7/StreamAssistant/releases) page to download
- Launch the aplication with the executable StreamAssistant.exe
- Make sure you have the right directories set up:

In the first time you open the application, go for Settings -> set directories and just save the default ones.

In the config directory you have all the folders to add more stuff and also all the files you need to import as OBS sources!

On the Colors directory you can add colors for Ranked, Splatfest and Turf War

- For each color group, you can have 3 different set of images: Alpha, containing the color of the Alpha team, Beta, containing the color for the Beta team and combo, containing colors for both of them
- On the alpha, beta and combo directories, you can also have any amount os subdirectories containing other images, as long as they all have the same name, they'll be created and updated for you to add them as OBS sources
- Reversing the colors will reverse the combo image vertically and horizontally, so more complicated shapes might not work very well.
- For images in subdirectories, they'll be found in the same importing directory as the ones in the root folder with their names starting with their directory's name.

- The Default directory contains team logos that are randomized when you insert a team name that doesn't have a matching team logo on the Teams directory.
- Regions are not currently images, a region is only a name for now, but this will be changed on the next release.
- In the importing directory you have a folder for each section of the Stream Helper, so it's easier to find the correct importing file.

Images in the importing directory don't have any format so that you can add them as OBS sources no matter if the content will be a png or jpg file (for now, those are the only image formats available, i will probably add more in the next release)

In the importing files directory there's also a full OBS layout, with lots of sources configured in the importing files directory for you to import into your own OBS. Just make sure to assign the correct files on each source, the [video tutorial](https://www.youtube.com/watch?v=aRtqBaEw-TA) has a section on this starting at 25:26.

Feel free to add issues in here as i plan to keep adding more Feature and qol changes for this project, you can also DM me [@Guigas_Jr](https://twitter.com/Guigas_Jr)

# Planned Releases

### V1.5 - Planned for October

- Support for more image file types
- Linux and MAC version
- Support region images

### V2.0 - Planned for January 2022

- Script to download (and resize) all images from a Battlefy tournament
- Adding Teams with Unicode Names (currently team names are restricted to valid file names);
- Translations;

### V3.0 - Not planned

- Dumping all data on json and making Javascript functions to get all data on html templates
- Triggering OBS Scene Changes
