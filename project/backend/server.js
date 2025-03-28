import express from 'express';
import net from 'net';
import path from 'path';
import cors from 'cors';

const app = express();
app.use(cors());

const SERVER_PORT = process.env.SERVER_PORT || 3000;
const CPP_HOST = process.env.CPP_SERVER_HOST || 'localhost';
const CPP_PORT = parseInt(process.env.CPP_SERVER_PORT || '5000');

// Serve static files (the web client) from a "public" directory
app.use('/', express.static(path.join(__dirname, 'public')));

// Endpoint: /random?n=
app.get('/random', (req, res) => {
  const n = parseInt(req.query.n);
  if (!n || isNaN(n)) {
    return res.status(400).json({ error: 'Invalid or missing n' });
  }

  // Connect to the C++ server via TCP
  const client = new net.Socket();

  client.connect(CPP_PORT, CPP_HOST, () => {
    // Send the message "random n"
    client.write(`random ${n}`);
  });

  let dataBuffer = '';
  
  client.on('data', data => {
    dataBuffer += data.toString();
  });

  client.on('end', () => {
    // The C++ server ends with a newline
    // The format is "val1,val2,val3,...\n"
    const trimmed = dataBuffer.trim();
    // Split on comma
    const stringNumbers = trimmed.split(',');
    const numbers = stringNumbers.map(numStr => parseFloat(numStr));
    res.json(numbers);
  });

  client.on('error', err => {
    console.error('Error connecting to C++ server:', err);
    res.status(500).json({ error: 'Error connecting to C++ server' });
  });
});

// Start the server
app.listen(SERVER_PORT, () => {
  console.log(`Express server running on port ${SERVER_PORT}`);
});
