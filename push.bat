@echo off
git add .
git commit -m "A place further than the Universe"
eval `ssh-agent -s`
ssh-add D:/github/ssh_key/id_ed25519
git push