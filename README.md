# Splatoon Stream Assistant

This is a program designed to assist streamers that want to use **dynamic data** on **OBS,** like a scoreboard, maplist or team data, but don't know / don't want to use **html** templates.

You can use **your own layout images** and add **your own team logos**, **map images**, **team colors** and even **mode icons** to customize your stream. The assistant keeps track of each team's name, logo, score, color and region + maps, modes, players, weapons and winners for a round up to a BO9. You can also add messages, titles, a logo, name and other data for your casters and change it all anytime with this Stream Assistant.

# Getting Started

- **Download the application (**Go to the [Releases](https://github.com/guigas7/StreamAssistant/releases) page for more options)**.**
    - Download the program **[Splatoon.Stream.Assistant.-.publish.rar](https://github.com/guigas7/StreamAssistant/releases/download/stable/Splatoon.Stream.Assistant.-.publish.rar).**
    - If you already have an **older** version, you can download only the .exe file, put it in the same folder as the older one, replacing the executable file **[StreamAssistant.exe](https://github.com/guigas7/StreamAssistant/releases/download/stable/StreamAssistant.exe).** You’ll still need to follow the next steps to configure the data directiories.
- Extract the .rar file and launch the aplication with the executable **StreamAssistant.exe.**
- Click on **Settings** -> **Set Directories**. These are the folders the program will use to **find and save data.** In there you can choose **custom** directories or just save the **default** ones (which are all in the config folder). **Make sure to hit the save button**.

# Importing Files in OBS

The files you’ll **import** in **OBS** are all in the directory you set as the **Importing Files Directory**. Each file corresponds to the data inserted by an input field in the assistant.

The Stream Assistant reads options inserted as **text** (.txt) or **image** (.png / .jpg /.jpeg / .gif) files in the **config directories** and saves the inserted / selected data in the **files** inside the **importing directory** (you’ll **import** these files as text or image sources on **OBS**). **Images** in the **importing directory** won’t have the **name extension** so you can use different image **formats** and still import only **one** file in **OBS** (one team can have a **png** logo and another can have a **jpg** logo, and they’ll both work when you import the file “**logo**” in **OBS**).

The path to the importing directory (config/importingDirectory) will be reffered as .../ in this guide. These are the files you’ll have access to import into your OBS overlay, organized with subfolders containing the importing files for each section:

### .../casters

- 1Name.txt, 2Name.txt, 3Name.txt, 1Twitter.txt, 2Twitter.txt, 3Twitter.txt

### .../info

- message.txt, subtitle.txt, title.txt, twitter.txt, tournamentLogo.txt, tournamentLogo

### .../round

- color.txt, colorGroups.txt, colors*, [subfolder]_colors**, round.txt, teamNames.txt***

<aside>
💡 * colors is the optional color you can set in the combo color directory, in case you have in one image the color for both teams.

</aside>

<aside>
💡 ** By adding subfolders in the colors directories you can have multiple color images, and they’ll be named after the subdirectory they’re in.

</aside>

<aside>
💡 *** teamNames.txt is a file that contains both team names in one line, separated by “ - “. This file is generated since OBS doesn’t allow for a text source combining 2 files.

</aside>

### .../set

- /**maps (maps name and image for each round)**
    - [1 - 9][.txt]
- /**modes (mode name and icon for each round)**
    - [1 - 9][.txt]
- /**winners (winner team name and logo for each round)**
    - [1 - 9][.txt]
- /**winpoints (winpoints image for each round)**
    - [1 - 9]_[winpoint name]
- /**weapons (Weapons name and image for rounds 1 - 9 and both teams [Alpha 1 - 4, Bravo 5 - 8])**
    - R[1 - 9]_[1 - 8], R[1 - 9]_[1 - 8].txt
- /**players (Players names for rounds 1 - 9 and of both teams [Alpha 1 - 4, Bravo 5 - 8])**
    - R[1 - 9]_[1 - 8]
- currentGame.txt, currentMap, currentMap.txt, currentMode, currentMode.txt, currentWinner, currentWinner.txt, Weapon[1-8], Weapon[1-8].txt, Player[1-8].txt

### .../teamAlpha / teamBravo

- color, color.txt, logo, name.txt, score.txt, region.txt, [subfolder]_color*, [customFile]**, [customFile].txt**

<aside>
💡 * By adding subfolders in the colors directories you can have multiple color images, and they’ll be named after the subdirectory they’re in.

</aside>

<aside>
💡 ** You can add any amount of custom image or text files, with any name you want. It’s recommended to use the same file name in all team folders so you can import just one file in OBS to show each team data.

</aside>

# Teams

Teams can be added in the TeamsDirectory by placing an image file or a folder.

The easiest way to add a new team is to add an image file, the image will be the team logo and the file name will be the team name (it ignores the extension).

If you add a folder instead of an image, the name of the folder will be the name used in the assistant’s auto complete, but you can override the team name with a text file named name.txt inside that folder. The logo of the team has to be in a file named logo.[png / jpg / jpeg / gif] inside the team’s folder.

You can also have any custom text / image data in a team folder, that will be copied to the importing directory when the team is selected. For example, you can have a text file teamMembers.txt in all you teams containing the name of the players of each team, or teamBanner.[png / jpg / jpeg / gif]. You can add any text / image files with any names you want, the importing file will have the same name as the custom file in the team folder.

<aside>
💡 It’s possible to assign different folders for team Alpha and Bravo, then you can have the same team with different data depending if they’re selected as Alpha / Bravo team. Although if you just plan to fit team’s logo in different shapes, an easier way is to just apply a mask filter in the logo source on OBS.

</aside>

If you input a team name that doesn’t have any folder / image matching in the teams folder, the Assistant will pick an image from the logo folder in the default directory.

### Importing team data from Battlefy

In the root folder of the project, there’s a folder named "battlefy-api-master” with 2 programs that can download team data from battlefy tournaments.

To choose tournaments to download data, just put their ID in the TournamentIDs.txt file. If you want to download team data from more than one tournament, put one ID in each line (data will be downloaded in order and overwriten if a team with the same name appears in more than one tournament). A tournament ID is the section between two slashes, right after the tournament name, in the URL: https://battlefy.com/inkling-performance-labs/swim-or-sink-65/624a51f17679cc0617610f6b/info

To download team data just run one of the two programs:

- **only_imgs.exe**
    - Downloads only team images from all signed up teams with custom logos.
- **teamDataDownload**
    - Downloads team image, name and players from all signed up teams. Players are saved in custom text files as member[1-99].txt, teamMembers.txt and teamMembersInline.txt

The script limits the team name to 120 characters because some operating systems have a limit on file path size of 255 (meaning the full path of the file can’t go over that or it won’t find the file).

# Maps and Modes

On the Maps directory you can add each map image. The name of the image will be the name of the map (it ignores the extension). Same goes for the modes. The program saves image and name of all 9 selected maps and modes in ...set/maps/, ...set/modes/.

Maps and modes from the current selected game are also saved in ...set/

# Players and Weapons

Similar to Maps and Modes, each game can have up to 8 players and 8 weapons (1 - 4 for Alpha, 5 - 8 for bravo). Weapons will save image and name from files in the weapons directory. The players select box will be filled from the content of files named member[1-99].txt in the selected team’s folder. You can also type in the select box so you don’t need to add the team’s players beforehand.

By double clicking the label of the first game in the players tab, if all the fields from that game are empty it will fill the fields with the first players from files member1.txt to member4.txt, if they exist.

![Players1](Splatoon%20Stream%20Assistant%2095e4e81d4e404c7994af32a5d5d1977d/Players1.png)

![Players2](Splatoon%20Stream%20Assistant%2095e4e81d4e404c7994af32a5d5d1977d/Players2.png)

To clear the players / weapons selected for a team. There’s a button in the bottom of the section to clear all games, or you can double click a game label to clear the data only for that game and team.

If there’s no weapon / player selected for that game, double clicking the game label will copy the selection for the previous game.

![Players3](Splatoon%20Stream%20Assistant%2095e4e81d4e404c7994af32a5d5d1977d/Players3.png)

![Players4](Splatoon%20Stream%20Assistant%2095e4e81d4e404c7994af32a5d5d1977d/Players4.png)

# Colors

On the Colors directory you can add colors for each colorset (Ranked, Turf war, Splatfest)

The colors have a set order that the game follows once a lobby is opened. If you order the image files for the colors correctly (like the example below) and set the auto update colors, you’ll only need to select the color for the first game of the set, and the next color will be selected once the round is changed.

![Colors1](Splatoon%20Stream%20Assistant%2095e4e81d4e404c7994af32a5d5d1977d/Colors1.png)

![Colors2](Splatoon%20Stream%20Assistant%2095e4e81d4e404c7994af32a5d5d1977d/Colors2.png)

![Colors3](Splatoon%20Stream%20Assistant%2095e4e81d4e404c7994af32a5d5d1977d/Colors3.png)

- For each color group you can have 3 different set of images:
    - alpha: For images containing the color for the Alpha team
    - bravo: for images containing the color for the Bravo team
    - combo: For images containing the colors for both teams
- In each directory you can have a set of images for each possible color. The set of images for Alpha, Bravo and combo need to have the same file names. For example, you’ll have 3 different images, in alpha, bravo and combo directories for blue vs green, but each of them will have the same name:
    - ColorsDirectory/RankedColorsDirectory/alpha/BluevsGreen.png (image containing the blue color)
    - ColorsDirectory/RankedColorsDirectory/bravo/BluevsGreen.png (image containing the green color)
    - ColorsDirectory/RankedColorsDirectory/combo/BluevsGreen.png (image containing blue and green color)
- On the alpha, bravo and combo directories, you can also have any amount os subdirectories containing other set of images, as long as they all have the same name, they'll be created and updated with the name [subfolder name]_color in the same importing folder as the main color file.
- Reversing the colors will reverse the combo image vertically and horizontally. Some complicated shapes might not work very well with this feature.

Feel free to add issues in here for bugfixes or asking new features. You can also DM me [@Guigas_Jr](https://twitter.com/Guigas_Jr)

# Logo

You can use this field to select between images stored in the Logos directory. 

# Winpoints

Winpoints are images that you can set for when a team has won a game. You can have different winpoints for bravo and alpha. You can also have as many winpoints as you want, they’ll be copied to the importing folder with the winpoint filename (ignoring extension).

<aside>
💡 A common use of winpoints is to put a dark image (with less than 100% opacity) over a map in the round screen, to indicate that the map has already been played out.

</aside>

# Custom Features:

I’m not currently working on new features, but i’m keeping track of user feedback on things that could be usefull in the application. If you have any suggestions / requests, you can send me a dm on discord Guigas#4774 or on twitter [@Guigas_Jr](https://twitter.com/Guigas_Jr).

I’m not being paid to do this, so don’t expect the next release to come out anytime soon. (Unless you want to pay me 👀).

### Possible next features:

- Adding custom caster data (to use with pronouns, picture or anything else).
- Adding reversed combo colors folder (so you can configure the program to use that instead of mirroring the image, which doesn’t work well depending on the shape of your color image).
- Vertical layout (it’s just positioning the tabs vertically, but believe me, it’s a pain in the ass to make it work with the way the program was made).
- Saving data in json format to use the program with html / javascript overlays.