# Containerizing

This file is meant to be read from top to bottom. Some things will be explained in sections only once and never again.

## Cpp Server

The first line is:
```dockerfile
FROM ubuntu:latest
```
Which means that we are using the Ubuntu image and we are using the latest Ubuntu. You can use different type of Linux distros but we chose Ubuntu for this example.
<br/>
This essentially sets up the Linux environment for us and let's us run Linux commands and Linux specific code (even if we are on Windows which is great).

The second line is:
```dockerfile
RUN apt-get update && apt-get install -y \
    build-essential \
    && rm -rf /var/lib/apt/lists/*
```
`RUN` means to run a command and the command we are running is `apt-get` to update and install all necessary packages like `build-essentials`. We are also removing the `/var/lib/apt/lists/` folder's content to have a smaller container size (as `update` stores the list of packages in that folder).

*Note.* When using Linux it is always recommended to run update before installing anything.

*Note.* With a Linux image, we are starting as root, that's why you don't see `sudo`. We don't need users for this specific implementation but if you do, that would be part of your `Dockerfile`.

The next few lines copy our code file into the container, set the work directory into `/app` and then compile our code resulting in the binary being named `server`.
```Dockerfile
COPY main.cpp /app/main.cpp
WORKDIR /app
RUN g++ main.cpp -o server
```
Notice that `WORKDIR` is a similar command to `cd` however it changes the folder the container is looking at rather than change the directory temporary to run a specific command. The last two lines could have been replaced with a `RUN` command instead as such:
```Dockerfile
RUN cd /app && g++ main.cpp -o server
```
But after that we are back to the root directory.

Lastly we expose the default port the server will run on (essentially saying, allow access to the container through that port) and we are running our server using `CMD`:
```Dockerfile
EXPOSE 5000
CMD ["./server"]
```

*Note.* We provide the `CMD` line an array of our command we want to run. If we have a few arguments, we would have separated them using commands as such `CMD ["./server", "--no-debug"]`.

*Note.* The `RUN` command is used to specify commands when we are building our container. The `CMD` command on the other hand specifies what command will be ran whenever the container starts (as we will build the container once and run it many times).

## Backend Server

*Note.* In this `Dockerfile` you will see we specify the folder `backend/` as if we are running the file from outside the folder and from the `project/` folder. That is because we are. We will elaborate on that when we talk about the `docker-compose.yml` file.

```Dockerfile
FROM node:14

WORKDIR /usr/src/app
```
From the get go we use the `node:14` container for our project. You can use `node:latest` but as node changes you may not want to and your code may only run on previous version. This is another great thing for our application.

We set the working directory as well to be the folder will put all of our files in. Notice that `/usr/src/` is an arbitrary choice but that's the best practice currently to do so.

```Dockerfile
COPY backend/package.json /usr/src/app/
COPY backend/package-lock.json /usr/src/app/

RUN npm install
```
We then proceed to copy our `package` files and running `npm install` to install all of our libraries.

```Dockerfile
COPY server.js /usr/src/app/
COPY frontend /usr/src/app/public
```
We now copy our `server.js` file, and copy the `frontend/` folder renaming it to be `public/` as we know we will require it.

```Dockerfile
EXPOSE 3000
CMD ["npm", "start"]
```
Then we expose the port `3000` and run our code using `npm start`.

**Important.** We are first copying the `package` files, installing and then copying the rest of our files. Some might wonder why not just copy everything. The reason is because Docker works in layers. This means that Docker will not run lines that didn't their output didn't change (or the output of previous files).
<br/>
Because it is less common for the `package` files to change, unlike our code files or our frontend. We don't want to have to install our packages every single time we change our code and want to see the changes. That's why the files that are more likely to change are placed later in the file.

## Docker Compose

We've created all of our necessary files to create each container on it's own. Now let's create a `docker-compose.yml` file to combine everything into a single command.

The first line specifies the version for backwards compatibility. We can ignore that line.

Each container is a service in our `docker-compose.yml` and this file is a `.yml` file so whitespaces matter. Make sure no trailing whitespaces exist in your file.

We declare two services, `server` and `backend`.

We declare the service's name using `server:` and then we tell it how to build. In our case we specify that it should build from the `./server` folder and it should build using the `Dockerfile` provided in the folder.
```yml
server:
  build:
    context: ./server
  dockerfile: Dockerfile
```
After that we specify a name for the container, and we set our environment variables.
```yml
server:
  ...
  container_name: server
  environment:
    - CPP_SERVER_PORT=5000
```
Then we specify that (left) port 5000 on our computer should map to (right) port 5000 on the container (which matches with our backend trying to run on that port).
```yml
server:
  ...
  ports:
    - "5000:5000"
```

And that's it for the C++ server.

For our backend we mostly have the same lines however, notice how the context is different.
```yml
backend:
  build:
    context: ./
    dockerfile: backend/Dockerfile
  container_name: backend
  environment:
    - SERVER_PORT=3000
    - CPP_SERVER_HOST=server
    - CPP_SERVER_PORT=5000
  ports:
    - "3000:3000"
  depends_on:
    - server
```
We specify that the context is the `project/` directory, that is because the context tells the `Dockerfile` what files it can actually access. Since we know that our `backend/Dockerfile` will have to access the frontend folder, we specify that in the context and in the actual `Dockerfile` with the path specifications.

Also notice that we tell the backend that it `depends_on: server` meaning that this container needs to have the server container running before it can run. That way we can tell Docker Compose exactly the order we want to run our containers.

One last thing, which I find amazing, is that we specify in the `environment` that `CPP_SERVER_HOST=server`. In our code the variable `CPP_SERVER_HOST` is the url for the C++ server. Specifying that it will be `server` is like saying "the url is the localhost of the `server` container, so just figure that out for us".
<br/>
I have seen many times where bad Docker instructions tell the user to run a command to find what is the address the user should put. **We don't need that**. Docker gives aliases for our networks and we can just use the **name of the service**.

With everything, we can now run:
```bash
docker compose up
```
In development I like running the following command instead:
```bash
docker compose up --build -d
```
where `--build` forces Docker Compose to build our images even if they are already built and `-d` runs the container in detach mode allowing me to not have a terminal dedicated for a container (sometimes I have a lot of terminals open so that helps with clutter).

## Purging Docker

Sometimes, we want to be able to delete all of docker's memory and start from scratch. There are a few commands that we will need to run in order to do that and you can stop the commands in whatever step you want. The commands are as follow:
```bash
# You will need to run most commands here as sudo if they fail do to "permission denied".

# Stop all running containers, will error if there are no running containers.
docker stop $(docker ps -q)

# Remove all containers, will error if there are no containers.
docker rm $(docker ps -aq)

# Remove all images, will error if there are no images.
docker rmi -f $(docker images -q)

# Remove all volumes, will error if there are no volumes.
docker volume rm $(docker volume ls -q)

# Remove all networks except the default ones, will error if there are no networks but the default ones.
# Note that you cannot really remove the default networks without breaking docker.
docker network rm $(docker network ls | grep -v "\bbridge\b\|\bhost\b\|\bnone\b" | awk '{print $1}')

# Prune the build cache
docker builder prune -af

# Ensure no residuals remain
docker system prune -af --volumes
```
Where the `-q` makes commands return just the IDs making our commands run on all of the IDs.

If you want to create a script file that has these commands for you but you don't want it to print error, just add `2>/dev/null` to any Docker command that can error.

And that completes this tutorial. If you need to reference it later you can ⭐ it!
<br/>
Thank you for reading!.
