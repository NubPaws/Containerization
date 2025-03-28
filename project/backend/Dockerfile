# Use an official Node.js image
FROM node:14

# Create app directory
WORKDIR /usr/src/app

# Copy package.json and package-lock.json
COPY package.json /usr/src/app/
COPY package-lock.json /usr/src/app/

# Install dependencies
RUN npm install

# Copy the server code
COPY server.js /usr/src/app/

# Copy the web client into the "public" folder we serve statically
# We'll do this by copying from the build context's "web-client" folder into "public"
COPY ../frontend /usr/src/app/public

# Expose port 3000
EXPOSE 3000

# Start the app
CMD ["npm", "start"]
