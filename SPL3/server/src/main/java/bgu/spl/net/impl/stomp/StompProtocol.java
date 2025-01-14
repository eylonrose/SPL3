package bgu.spl.net.impl.stomp;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImpl;


public class StompProtocol implements StompMessagingProtocol<String> {
    private boolean shouldTerminate = false;
    private int connectionId;
    private Connections<String> connections;
    @Override
    public void start(int connectionId, Connections<String> connections) {
        this.connectionId = connectionId;
        this.connections = connections;

    }

    @Override
    public void process(String message) {
        String[] parts = message.split("\n");
        String command = parts[0]; 

        switch (command) {
            case "CONNECT":
                handleConnect(parts);
                break;
            case "SUBSCRIBE":
                handleSubscribe(parts);
                break;
            case "SEND":
                handleSend(parts);
                break;
            case "DISCONNECT":
                handleDisconnect(parts);
                break;
            default:
                handleError("Unknown command: " + command);
        }
    }

    private void handleConnect(String[] parts) {
        connections.send(connectionId, "CONNECTED\nversion:1.2\n\n\u0000");
    }

    private void handleSubscribe(String[] parts) {
        String channel = extractHeader(parts, "destination");
        connections.subscribe(connectionId, channel);
        //send(String channel, T msg)
        connections.send(connectionId, "RECEIPT\nreceipt-id:123\n\n\u0000");
    }

    private void handleSend(String[] parts) {
        String channel = extractHeader(parts, "destination");
        String body = parts[parts.length - 1];
        connections.send(channel, body);
    }

    private void handleDisconnect(String[] parts) {
        shouldTerminate = true;
        connections.disconnect(connectionId);
        connections.send(connectionId, "RECEIPT\nreceipt-id:456\n\n\u0000");
    }

    private void handleError(String errorMessage) {
        connections.send(connectionId, "ERROR\nmessage:" + errorMessage + "\n\n\u0000");
    }

    private String extractHeader(String[] parts, String header) {
        for (String part : parts) {
            if (part.startsWith(header + ":")) {
                return part.substring(header.length() + 1).trim();
            }
        }
        return null;
    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }
    
}
