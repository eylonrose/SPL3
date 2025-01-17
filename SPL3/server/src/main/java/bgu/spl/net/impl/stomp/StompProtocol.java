package bgu.spl.net.impl.stomp;
import java.util.concurrent.ConcurrentHashMap;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImpl;


public class StompProtocol implements StompMessagingProtocol<String> {
    private boolean shouldTerminate = false;
    private int connectionId;
    private ConnectionsImpl<String> connections;
    private ConcurrentHashMap<User, Boolean> users = new ConcurrentHashMap<>();
    /*@Override
    public void start(int connectionId, ConnectionsImpl<String> connections) {
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
    }*/
    @Override
    public void start(int connectionId, ConnectionsImpl<String> connections) {
        this.connectionId = connectionId;
        this.connections = connections;
    }

    @Override
    public String process(String message) {
        String[] parts = message.split("\n");
        String command = parts[0];

        switch (command) {
            case "CONNECT":
                return handleConnect(parts);
            case "SUBSCRIBE":
                return handleSubscribe(parts);
            case "SEND":
                return handleSend(parts);
            case "DISCONNECT":
                return handleDisconnect(parts);
            case "UNSUBSCRIBE":
                return handleUnsubscribe(parts);
            default:
                return handleError("Unknown command: " + command, parts);
        }
    }

    private String handleConnect(String[] parts) {
        
        String username = extractHeader(parts, "login");
        String password = extractHeader(parts, "passcode");
        if(users.get(new User(username, password))){
            handleError("User already logged in", parts);
            return null;
        }
        for(User user : users.keySet()){
            if(user.getUsername().equals(username) && !user.getPassword().equals(password)){
                handleError("Wrong password", parts);
                return null;
            }
        }
        connections.send(connectionId, "CONNECTED\nversion:1.2\n\n\u0000");
        return "CONNECTED\nversion:1.2\n\n\u0000";  // Response to be sent
    }

    private String handleSubscribe(String[] parts) {
        String channel = extractHeader(parts, "destination");
        connections.subscribe(connectionId, channel);
        return "RECEIPT\nreceipt-id:"+ extractHeader(parts, "receipt")+"\n\n\u0000";  // Response to be sent
    }

    private String handleSend(String[] parts) {
        String channel = extractHeader(parts, "destination");
        String body = parts[parts.length - 1];
        connections.send(channel, body);
        return null;
    }

    private String handleDisconnect(String[] parts) {
        shouldTerminate = true;
        connections.disconnect(connectionId);
        return "RECEIPT\nreceipt-id:456\n\n\u0000";  // Response
    }

    private String handleError(String errorMessage, String[] parts) {
        return "ERROR\nmessage:" + errorMessage + getParts(parts) +"\n\u0000";
    }

    private String handleUnsubscribe(String[] parts) {
        String channel = extractHeader(parts, "destination");
        connections.unsubscribe(connectionId, channel);
        return "RECEIPT\nreceipt-id:"+ extractHeader(parts, "receipt")+"\n\n\u0000";  // Response to be sent
    }

    private String extractHeader(String[] parts, String header) {
        for (String part : parts) {
            if (part.startsWith(header + ":")) {
                return part.substring(header.length() + 1).trim();
            }
        }
        return null;
    }

    private String getParts(String[] parts){
        String ans = "";
        for (String part : parts) {
            ans = ans + part + "\n";
        }
        return ans;
    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }
    
}
