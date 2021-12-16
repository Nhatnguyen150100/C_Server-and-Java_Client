import java.io.*;
import java.net.*;
import java.util.Objects;
import java.util.Scanner;


class ClientJava {
    private void menuChoice(int choice, Socket socket) throws IOException {
        Scanner scanner = new Scanner(System.in);
        System.out.println("Welcome to PC COVID");
        System.out.println("Do you have an account? Let login to PC covid or sign in if you don't have an account:");
        System.out.println("1: Login");
        System.out.println("2: Sign in");
        System.out.print("Your choice: ");
        choice = scanner.nextInt();
        switch (choice) {
            case 1 -> {
                sendAccountMessage(String.valueOf(choice),socket);
                InputStream istream = socket.getInputStream();
                BufferedReader receiveRead = new BufferedReader(new InputStreamReader(istream), 1024);
                String accountName = null;
                String accountPassword = null;
                System.out.print("Name: ");
                input(socket, receiveRead, accountName, accountPassword,choice);
            }
            case 2 -> {
                sendAccountMessage(String.valueOf(choice),socket);
                InputStream istream = socket.getInputStream();
                BufferedReader receiveRead = new BufferedReader(new InputStreamReader(istream), 1024);
                String accountName = null;
                String accountPassword = null;
                System.out.println("Create a account:\n");
                System.out.print("NameAccount: ");
                input(socket, receiveRead, accountName, accountPassword,choice);
            }
            default -> {
                System.out.println("Error: unachievable");
            }
        }
    }

    private void input(Socket socket, BufferedReader receiveRead, String accountName, String accountPassword, int choice) throws IOException {
        String receiveMessage = null;
        String accountMessage = null;
        try {
            accountName = sendAccount(socket);
        } catch (IOException e) {
            System.out.println("error send account name message");
            e.printStackTrace();
        }
        System.out.print("Password: ");
        try {
            accountPassword = sendAccount(socket);
        } catch (IOException e) {
            System.out.println("error send account password message");
            e.printStackTrace();
        }
        accountMessage = createAccountMessage(accountName,accountPassword);

        sendAccountMessage(accountMessage, socket);
        receiveMessage = String.valueOf(receiveRead.readLine());
        receiveMessage = removeNonAscii(receiveMessage);
        receiveMessage = replaceUnreadable(receiveMessage);
        if(!Objects.equals(receiveMessage, "0")) //receive from server
        {
            System.out.print("from server: ");
            System.out.println(receiveMessage); // displaying at DOS prompt
        }
    }


    private String sendAccount(Socket sock) throws IOException {
        String sendMessage;
        BufferedReader keyRead = new BufferedReader(new InputStreamReader(System.in));
        sendMessage = keyRead.readLine();  // keyboard reading
        return sendMessage;
    }

    private String createAccountMessage(String name, String password){
        return name+ "_"+password;
    }


    private void sendAccountMessage(String str, Socket sock) throws IOException {
        OutputStream ostream = sock.getOutputStream();
        PrintWriter pwrite = new PrintWriter(ostream, true);
        pwrite.println(str);       // sending to server
        pwrite.flush();            // flush the data
    }


    public static void main(String[] args) throws Exception {
        int choice = 0;
        Socket sock = new Socket("127.0.0.1", 9999);
//        BufferedReader keyRead = new BufferedReader(new InputStreamReader(System.in));
//        OutputStream ostream = sock.getOutputStream();
//        PrintWriter pwrite = new PrintWriter(ostream, true);


        ClientJava clientJava = new ClientJava();
        clientJava.menuChoice(choice, sock);

//        String sendMessage;
//        while(true)
//        {
//            System.out.print("from client: ");
//            sendMessage = keyRead.readLine();  // keyboard reading
//            pwrite.println(sendMessage);       // sending to server
//            pwrite.flush();                    // flush the data
//        }

    }
    private static String removeNonAscii(String s){
        StringBuffer sb = new StringBuffer();
        for(int i=0; i<s.length(); ++i){
            if(s.charAt(i) < 128){
                sb.append(s.charAt(i));
            }
        }
        return sb.toString();
    }
    private static String replaceUnreadable(String s){
        return s.replaceAll("\\P{Print}", "");
    }
}