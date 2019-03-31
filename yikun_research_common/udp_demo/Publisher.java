import java.net.*;
import java.io.*;

public class Publisher {
    public static void main(String[] args) throws IOException {
        InetAddress address = InetAddress.getByName("127.0.0.1");
        int port = 20000;
        DatagramSocket socket = new DatagramSocket();
        byte[] data = "tag TID = E2801105200076526F9A0979\0".getBytes();
        DatagramPacket packet = new DatagramPacket(data, data.length, address, port);
        socket.send(packet);  /*publish*/
        socket.close();
    }
}
