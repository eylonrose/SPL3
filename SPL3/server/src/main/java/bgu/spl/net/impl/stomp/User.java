package bgu.spl.net.impl.stomp;

import java.util.concurrent.ConcurrentHashMap;

public class User implements Comparable<User> {
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

    @Override
    public int compareTo(User o) {
        if(this.username.equals(o.username) && this.password.equals(o.password))
            return 0;
        return 1;
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

}