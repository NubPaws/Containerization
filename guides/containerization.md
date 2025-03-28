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

*Note.* We provide the `CMD` line an array of our command we want to run. If we have a few arguments, we would have seperated them using commands as such `CMD ["./server", "--no-debug"]`.

*Note.* The `RUN` command is used to specify commands when we are building our container. The `CMD` command on the other hand specifies what command will be ran whenever the container starts (as we will build the container once and run it many times).

## Backend Server

```Dockerfile
FROM node:14

WORKDIR /usr/src/app
```
From the get go we use the `node:14` container for our project. You can use `node:latest` but as node changes you may not want to and your code may only run on previous version. This is another great thing for our application.

We set the working directory as well to be the folder will put all of our files in. Notice that `/usr/src/` is an arbitrary choice but that's the best practice currently to do so.

```Dockerfile
COPY package.json /usr/src/app/
COPY package-lock.json /usr/src/app/

RUN npm install
```
We then proceed to copy our `package` files and running `npm install` to install all of our libraries.

```Dockerfile
COPY server.js /usr/src/app/
COPY ../frontend /usr/src/app/public
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

For our backend we mostly have the same lines however
