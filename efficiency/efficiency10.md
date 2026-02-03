# Efficiency 10
# Install Sioyek - A Vim-like PDF Viewer

## Author: Ruinan Lin
## Date: Feb 3, 2026

### Problem Description

Since I've successfully configured my browser to use Vimium C, it seems that everything is good and I don't need my mouse any more, except for my PDF viewer. When I use the default PDF viewer provided by Ubuntu, I have to use my mouse again to move around.

This time, I find [Sioyek](https://github.com/ahrm/sioyek), which is a Vim-like PDF viewer.

### Install Sioyek

1. Go to the [Releases page](https://github.com/ahrm/sioyek/releases) and find a file called something similar to `sioyek-release-linux.zip`. Click and download it. I downloaded it into `~/Downloads/sioyek-release-linux.zip`.

2. Unzip it using command `unzip ~/Downloads/sioyek-release-linux.zip -d ~/sioyek_temp`. After that, you will find an executable file called `Sioyek-x86_64.AppImage` under path `~/sioyek_temp`. This is an image that you can directly execute.

3. Put the executable file under `/opt` by command `sudo mkdir -p /opt/sioyek` and `sudo cp -r ~/sioyek_temp/* /opt/sioyek/`. This is standard practice for storing large third-party software on Linux.

4. Create a shortcut command by linking it to somewhere searched by `PATH`. The command should be `sudo ln -s /opt/sioyek/Sioyek-x86_64.AppImage /usr/local/bin/sioyek`.

5. Remove the temporary directory by `rm -r ~/sioyek_temp/`

After that, you can directly open the Sioyek tutorial by typing `sioyek` in your terminal.

### Set Sioyek as the Default PDF Viewer

1. Create a file `~/.local/share/applications/sioyek.desktop` and paste the following contents:

    ```
    [Desktop Entry]
    Name=Sioyek
    Exec=/usr/local/bin/sioyek %f
    Type=Application
    Terminal=false
    Icon=sioyek
    MimeType=application/pdf;
    ```

2. Update MIME: `xdg-mime default sioyek.desktop application/pdf`.

Everything is done and you can open a PDF file using Sioyek by just clicking on it. However, it seems a little bit ugly if Sioyek does not have its icon. The following step will give it an icon:

1. Download the [icon](https://github.com/ahrm/sioyek/blob/main/resources/sioyek-icon-linux.png) and put and rename it to `/usr/share/icons/hicolor/512x512/apps/sioyek.png`.

2. We've already set the correct `Icon` field in `~/.local/share/applications/sioyek.desktop`, so we don't need to do anything here.

3. Update the icon cache by `sudo gtk-update-icon-cache /usr/share/icons/hicolor`.

4. Update the desktop database by `update-desktop-database ~/.local/share/applications`.

After that, you should see the icon.

### Self Configurations

Sioyek's default configuration contains another set of operations that is not similar to Vim's or Vimium C's. So we need to add other configurations. To achieve this, we should create two configuration files: `~/.config/sioyek/keys_user.config`, which functions as `map`; and `~/.config/sioyek/prefs_user.config`, which functions as `set`.

In `~/.config/sioyek/keys_user.config`, I write the following configurations. The goal is to make the mapping rules as close as possible to those of Vim.

```
# Basic navigation
move_down					j
move_up						k
move_left					l
move_right					h
next_page					<C-f>
previous_page				<C-b>
screen_down					d
screen_up					u
goto_top_of_page			H
goto_bottom_of_page			L

# Go to page
goto_page_with_page_number	<S-g>

# Open table of contents
goto_toc					T

# Jump history
prev_state					<C-o>
next_state					<C-i>
```

I haven't figured out how things work in `~/.config/sioyek/prefs_user.config`. There are Chinese comments there and we do not record them here.
