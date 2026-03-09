#!/usr/bin/env bash
set -e

echo ">>> Configuring RainDropOS live environment"

# Set locale
sed -i 's/#en_US.UTF-8/en_US.UTF-8/' /etc/locale.gen
locale-gen

# Set timezone
ln -sf /usr/share/zoneinfo/UTC /etc/localtime

# Create raindrop user
useradd -m -G wheel,audio,video,input,storage -s /bin/bash raindrop

# Set default password — user changes this on first boot via installer
echo "raindrop:raindrop" | chpasswd

# Allow wheel group to use sudo without password
# This is for the live environment only — installer locks this down
sed -i 's/# %wheel ALL=(ALL:ALL) NOPASSWD: ALL/%wheel ALL=(ALL:ALL) NOPASSWD: ALL/' /etc/sudoers

# Copy skel files to raindrop home
cp /etc/skel/.bash_profile /home/raindrop/.bash_profile
cp /etc/skel/.xinitrc      /home/raindrop/.xinitrc
chmod +x /home/raindrop/.xinitrc
chown -R raindrop:raindrop /home/raindrop

# Enable services
systemctl enable NetworkManager
systemctl enable raindrop-daemon

# Create config directories
mkdir -p /home/raindrop/.config/raindrop/profiles
chown -R raindrop:raindrop /home/raindrop/.config

echo ">>> RainDropOS live environment configured"