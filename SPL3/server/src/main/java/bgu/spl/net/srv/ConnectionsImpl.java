package bgu.spl.net.srv;
import java.util.concurrent.*;

public class ConnectionsImpl<T> implements Connections<T> {
    private ConcurrentHashMap<Integer, ConnectionHandler<T>> connections = new ConcurrentHashMap<>();
    private ConcurrentHashMap<String, CopyOnWriteArrayList<Integer>> topics = new ConcurrentHashMap<>();
    private static ConnectionsImpl<?> instance = null;

    public static synchronized <T> ConnectionsImpl<T> getInstance() {
        if (instance == null) {
            instance = new ConnectionsImpl<>();
        }
        return (ConnectionsImpl<T>) instance;
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

    public void unsubscribe(int connectionId, String channel) {
        if (topics.containsKey(channel)) {
            topics.get(channel).remove(connectionId);
        }
    }
    
}
