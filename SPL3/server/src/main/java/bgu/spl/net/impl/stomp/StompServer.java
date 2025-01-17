package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.impl.stomp.StompEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.srv.BaseServer;
import bgu.spl.net.srv.Server;


public class StompServer {

    public static void main(String[] args) {
        int port = Integer.parseInt(args[0]);
        String serverType = args[1];
        BaseServer<String> server;

        if (serverType.equals("tpc")) {
            Server.threadPerClient(
                    port,
                    () -> new StompProtocol(),
                    () -> new StompEncoderDecoder()
            ).serve();
        } else if (serverType.equals("reactor")) {
            int nThreads = 8; 
            Server.reactor(
                    nThreads,
                    port,
                    () -> new StompProtocol(),
                    () -> new StompEncoderDecoder()
            ).serve();
        } else {
            System.out.println("Invalid server type. Use 'tpc' or 'reactor'.");
        }
    }
}
