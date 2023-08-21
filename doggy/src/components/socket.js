import io from 'socket.io-client';
const socket =
io('http://localhost:8001',
    { reconnectionDelay: 300, reconnectionDelayMax: 300, autoConnect:true });
export default socket