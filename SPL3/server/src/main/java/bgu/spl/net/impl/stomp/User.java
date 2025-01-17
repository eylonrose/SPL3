package bgu.spl.net.impl.stomp;

public class User implements Comparable<User> {
    private String username;
    private String password;

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

}