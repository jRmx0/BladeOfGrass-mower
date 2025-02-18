import { useState, useEffect } from 'react'
import './App.css'
import { Joystick } from 'react-joystick-component';

type JoystickDirection = "FORWARD" | "RIGHT" | "LEFT" | "BACKWARD";

export interface IJoystickUpdateEvent {
  type: "move" | "stop" | "start";
  x: number | null;
  y: number | null;
  direction: JoystickDirection | null;
  distance: number | null;
}

function App() {
  const [btnState, setBtnState] = useState(true);
  const [message, setMessage] = useState("");
  const [ledOn, setLedOn] = useState(false);
  const [socket, setSocket] = useState<WebSocket | null>(null);

  useEffect(() => {
    getHelloWorld();
    webSocket();
  }, []);

  const handleMove = (event: IJoystickUpdateEvent) => {
    console.log('Move event:', event);
    if (socket && socket.readyState === WebSocket.OPEN) {
        const joystickData = {
            type: 'joystick',
            action: 'move',
            x: event.x,
            y: event.y,
            direction: event.direction,
            distance: event.distance
        };
        socket.send(JSON.stringify(joystickData));
    }
  };

  const handleStop = (event: IJoystickUpdateEvent) => {
    console.log('Stop event:', event);
    if (socket && socket.readyState === WebSocket.OPEN) {
        const joystickData = {
            type: 'joystick',
            action: 'stop',
            x: 0,
            y: 0,
            direction: null,
            distance: 0
        };
        socket.send(JSON.stringify(joystickData));
    }
  };

  const getHelloWorld = async () => {
    const webResult = await fetch("/api/hello-world");
    const myText = await webResult.text();
    setMessage(myText);
  };

  const getWebSocketUrl = (suffix: string) => {
    const l = window.location;
    return ((l.protocol == "https:") ? "wss://" : "ws://") + l.host + l.pathname + suffix;
  }

  const webSocket = () => {
    const socket = new WebSocket(getWebSocketUrl("ws"))
    socket.onopen = () => {
      socket.send("Hello ESP32");
      setSocket(socket);
    }
    socket.onmessage = (event) => {
      console.log(event.data);
      if (event.data.startsWith('{')) {
        try {
          const attemptedBtnState = JSON.parse(event.data);
          setBtnState(attemptedBtnState.btn_state);
        } catch (err) {
          console.error('Failed to parse JSON:', err);
        }  
      }
    }
    socket.onerror = (err) => console.error(err);
    socket.onclose = (event) => console.log(event);
  };

  const switchLed = async (is_on: boolean) => {
    const payload = { is_on };
    const webResult = await fetch("/api/toggle-led", {
      method: "POST",
      body: JSON.stringify(payload)
    });
    if (!webResult.ok) {
      console.error(webResult.statusText);
      return;
    }
    setLedOn(is_on);
  }

  const getLedText = () => ledOn ? "LED is on" : "LED is off";

  

  return (
    <>
      <h1>{message}</h1>
      <div className="card">
        <button
          style={{ background: ledOn ? "blue" : "" }}
          onClick={() => switchLed(!ledOn)}
        >
          {getLedText()}
        </button>
        <div className={`component-btn-builtin-state ${btnState ? 'active' : ''}`}></div>
      </div>
      <div className="component-joystick">
        <Joystick
          size={100}
          baseColor="#ffac7f"
          stickColor="#ff833f"
          move={handleMove}
          stop={handleStop}
          throttle={50}
        />
      </div>
    </>
  )
}

export default App

