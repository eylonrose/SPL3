package bgu.spl.net.srv;
import java.util.concurrent.*;;

public class ConnectionsImpl<T> implements Connections<T> {
    private ConcurrentHashMap<Integer, ConnectionHandler<T>> connections;
    private ConcurrentHashMap<String, CopyOnWriteArrayList<Integer>> topics = new ConcurrentHashMap<>();

    public ConnectionsImpl() {
        connections = new ConcurrentHashMap<>();
    }

    @Override
    public boolean send(int connectionId, T msg) {
        ConnectionHandler<T> connection = connections.get(connectionId);
        if (connection != null) {
            connection.send(msg);
            return true;
        }
        return false;
    }

    @Override
    public void send(String channel, T msg) {
        for (ConnectionHandler<T> connection : connections.values()) {
            if (connection.getChannel().equals(channel)) {
                connection.send(msg);
            }
        }
    }

    @Override
    public void disconnect(int connectionId) {
        connections.remove(connectionId);
    }

    public void addConnection(int connectionId, ConnectionHandler<T> connectionHandler) {
        connections.put(connectionId, connectionHandler);
    }

    public void subscribe(int connectionId, String channel) {
        topics.putIfAbsent(channel, new CopyOnWriteArrayList<>());
        topics.get(channel).add(connectionId);
    }
    
}