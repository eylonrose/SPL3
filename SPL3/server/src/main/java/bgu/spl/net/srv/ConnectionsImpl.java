package bgu.spl.net.srv;
import java.util.concurrent.*;
import bgu.spl.net.impl.stomp.User;

public class ConnectionsImpl<T> implements Connections<T> {
    private ConcurrentHashMap<Integer, ConnectionHandler<T>> connections = new ConcurrentHashMap<>();
    private ConcurrentHashMap<String, CopyOnWriteArrayList<User>> topics = new ConcurrentHashMap<>();
    private ConcurrentHashMap<User, Boolean> users = new ConcurrentHashMap<>();
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
        if (topics.containsKey(channel)) {
            for (User user : topics.get(channel)) {
                if (users.get(user)) {
                    send(user.getConnectionId(), msg);
                }
            }
        }
    }

    @Override
    public void disconnect(int connectionId) {
        connections.remove(connectionId);
        for (User user : users.keySet()) {
            if(user.getConnectionId() == connectionId) {
                users.put(user, false);
            }
        }
    }

    public void addConnection(int connectionId, ConnectionHandler<T> connectionHandler, User user) {
        connections.put(connectionId, connectionHandler);
        users.put(user, true);
        for(User u : users.keySet()){
            if(u.equals(user)){
                u.setConnectionId(connectionId);
            }
        }
    }

    public void subscribe(int connectionId, String channel) {
        topics.putIfAbsent(channel, new CopyOnWriteArrayList<>());
        for (User user : users.keySet()) {
            if(user.getConnectionId() == connectionId) {
                if (!user.isSubscribed(channel)) {
                    user.addTopic(channel, topics.get(channel).size());
                    topics.get(channel).add(user);
                }
            }
        }
    }

    public void unsubscribe(int connectionId, String channel) {
        for (User user : users.keySet()) {
            if(user.getConnectionId() == connectionId) {
                if (user.isSubscribed(channel)) {
                    topics.get(channel).remove(user);
                    user.removeTopic(channel);
                }
            }
        }
    }

    public ConcurrentHashMap<User, Boolean> getUsers() {
        return users;
    }
    
}
