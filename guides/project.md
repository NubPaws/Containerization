# Project Layout

In this project we have a folder called `project/` that contains all of our project files. 

We have three folders:
- `server/`
  - `main.cpp`
  - `Dockerfile`
- `backend/`
  - `server.js`
  - `package.json`
  - `package-lock.json`
  - `Dockerfile`
- `frontend/`
  - `index.html`

*Note.* The `Dockerfile` is used to specify how to build a container. We will have two containers in this project as you can tell by the fact there are two `Dockerfile`s provided. We will then use `docker-compose.yml` to construct everything.

## What the Project Does

Our project is simple. We have a C++ TCP server that listens for commands from clients. When a client sends the message `random n` the server returns `n` random numbers drawn from a normal distribution.

Our backend has an RESTful API end point `GET /random?n=` which essentially creates a connection with the server, and asks for `n` random numbers using the `random n` command.

Our frontend has a text field for the user to choose a number, and by pressing a button the user calls `GET /random?n=` with the number given from the user. The frontend then displays the numbers received sorted.

We'll delve into more depth about the files in our projects now.

## C++ Server

If you tried to run the server on Windows you might get an issue, that is because the server requires the usage of sockets and it uses unix sockets. That's why we want to run our server on a unix environment. If you look at the first few lines of the main you may also see the lines
```cpp
const char* portEnv = std::getenv("CPP_SERVER_PORT");
int port = portEnv ? std::stoi(portEnv) : 5000; // default 5000
```
These lines take the `CPP_SERVER_PORT` environment variable and parse it to an int if it exists (for those who don't know C++ don't worry, we don't really care about the C++ code, we more care about the fact we can read the function names and understand them).

*Note.* If no port is provided we default to port `5000`. This is important as we always want to have defaults. We want to enable the user to customize, but if they want to just run our server, they should be able to do so. And if any port is taken, it will be up to them to free them. We will also want to specify such information in our `README.md` file.

To read about the C++ server's `Dockerfile` you may go over to our [containerization](./containerization.md#cpp-server) explanation.

## Backend

Our backend is as simple as it gets. You can view the two endpoints provided, but you don't need to.

Basically we have a `GET /random?n=` endpoint and a `GET /` endpoint which just presents our website that comes from the frontend folder, more specifically it's the `frontend/index.html` file.

If you do inspect the code (or try and run it) you will see that `server.js` requires a public folder. We'll create that folder using Docker and also populate it with the file from the `frontend/` folder.

For information on how we are doing this, browse on over to [containerization](./containerization.md#backend-server)

## Frontend

Our frontend doesn't have much to it. Just note that it tries to `GET` from the path `/random` meaning we are assuming that the frontend runs on our backend server.

No `Dockerfile` here as there is no need to run our frontend on a separate container. The backend container already runs our code and that's enough. This just demonstrates that creating containers requires a bit of understanding how our code works and communicates within itself.
