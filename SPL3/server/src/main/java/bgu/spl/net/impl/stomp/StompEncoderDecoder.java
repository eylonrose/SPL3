package bgu.spl.net.impl.stomp;

import java.nio.charset.StandardCharsets;
import java.util.Arrays;

import bgu.spl.net.api.MessageEncoderDecoder;

public class StompEncoderDecoder implements MessageEncoderDecoder<String> {
    private byte[] buffer = new byte[1 << 10]; 
    private int len = 0;

    @Override
    public String decodeNextByte(byte nextByte) {
         if (nextByte == '\u0000') { 
            String message = new String(buffer, 0, len, StandardCharsets.UTF_8);
            len = 0;
            return message;
        } else {
            if (len >= buffer.length) {
                buffer = Arrays.copyOf(buffer, len * 2);
            }
            buffer[len++] = nextByte;
            return null;
        }
    }

    @Override
    public byte[] encode(String message) {
        return (message + '\u0000').getBytes(StandardCharsets.UTF_8);
    }
    
}