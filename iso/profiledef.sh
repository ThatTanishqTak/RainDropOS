#!/usr/bin/env bash

iso_name="raindropos"
iso_label="RAINDROP_$(date --date="@${SOURCE_DATE_EPOCH:-$(date +%s)}" +%Y%m)"
iso_publisher="RainDropOS"
iso_application="RainDropOS"
iso_version="0.1.0"
install_dir="arch"
buildmodes=('iso')
bootmodes=('bios.syslinux.mbr' 'bios.syslinux.eltorito' 'uefi-x64.systemd-boot.esp' 'uefi-x64.systemd-boot.eltorito')
arch="x86_64"
pacman_conf="pacman.conf"
airootfs_image_type="squashfs"
airootfs_image_tool_options=('-comp' 'xz' '-Xbcj' 'x86' '-b' '1M' '-Xdict-size' '1M')
file_permissions=(
    ["/etc/shadow"]="0:0:400"
    ["/root"]="0:0:750"
    ["/root/customize_airootfs.sh"]="0:0:755"
)


---

**`iso/packages.x86_64`**

# Base system
base
base-devel
linux
linux-firmware
linux-headers
mkinitcpio

# Boot
grub
efibootmgr
os-prober

# Display
xorg-server
xorg-xinit
xorg-xrandr
mesa
vulkan-radeon
vulkan-intel
nvidia
nvidia-utils

# Audio
pipewire
pipewire-pulse
pipewire-alsa
wireplumber

# Shell dependencies
sdl2
sdl2_ttf
ttf-jetbrains-mono-nerd

# Steam
steam

# Installer
calamares
calamares-tools

# System utilities
networkmanager
sudo
nano
git
wget
curl
htop

# Fonts
noto-fonts
noto-fonts-cjk
noto-fonts-emoji


---

**`iso/airootfs/etc/hostname`**

raindropos


---

**`iso/airootfs/etc/locale.conf`**

LANG=en_US.UTF-8


---

**`iso/airootfs/etc/vconsole.conf`**

KEYMAP=us