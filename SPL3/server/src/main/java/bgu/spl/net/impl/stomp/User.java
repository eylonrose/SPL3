package bgu.spl.net.impl.stomp;

import java.util.Objects;
import java.util.concurrent.ConcurrentHashMap;

public class User{
    private String username;
    private String password;
    private int connectionId;
    private ConcurrentHashMap<String, Integer> topics = new ConcurrentHashMap<>();

    public User(String username, String password) {
        this.username = username;
        this.password = password;
    }

    public String getUsername() {
        return username;
    }

    public String getPassword() {
        return password;
    }

    public void setConnectionId(int connectionId) {
        this.connectionId = connectionId;
    }

    public int getConnectionId() {
        return connectionId;
    }

    public void addTopic(String topic, int subscriptionId) {
        topics.put(topic, subscriptionId);
    }

    public void removeTopic(String topic) {
        topics.remove(topic);
    }

    public boolean isSubscribed(String topic) {
        return topics.containsKey(topic);
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (obj == null || getClass() != obj.getClass()) return false;
        User user = (User) obj;
        return username.equals(user.username) && password.equals(user.password);
    }

    @Override
    public int hashCode() {
        return Objects.hash(username, password);
    }

}