./config.sh
git status
git add .
git commit -m "Initial Commit"
git push -u origin master
git ls-remote origin

read -n 1 -s

git swith -c main
echo "copy & paste the github repo url command"
echo "do not forget to change the remote name origin->public"
git push -u public main
git ls-remote public
echo "done! successfully set up project maintance tool"
