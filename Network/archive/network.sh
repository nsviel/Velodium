echo IA module interfacing initialization ...
sudo adduser obstacle
sudo mkdir -p /var/sftp/uploads
#Set the owner of /var/sftp to root.
sudo chown root:root /var/sftp
#Give root write permissions to the same directory, and give other users only read and execute rights.
sudo chmod 755 /var/sftp
#Change the ownership on the uploads directory to obstacle.
sudo chown obstacle:obstacle /var/sftp/uploads
#Restricting Access to One Directory
sudo nano /etc/ssh/sshd_config
echo Match User obstacle
echo ForceCommand internal-sftp
echo PasswordAuthentication yes
echo ChrootDirectory /var/sftp
echo PermitTunnel no
echo AllowAgentForwarding no
echo AllowTcpForwarding no
echo X11Forwarding no
#To apply the configuration changes, restart the service.
sudo systemctl restart sshd

