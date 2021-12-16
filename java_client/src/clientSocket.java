import inforUser.account;

import java.io.*;
import java.net.*;
import java.util.Objects;
import java.util.Scanner;


class ClientJava {
    private void menuChoice(Socket socket) throws IOException {
        int choice;
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
                System.out.print("Name: ");
                input(socket);
                checkOutPut(receiveRead, socket);
            }
            case 2 -> {
                sendAccountMessage(String.valueOf(choice),socket);
                InputStream istream = socket.getInputStream();
                BufferedReader receiveRead = new BufferedReader(new InputStreamReader(istream), 1024);
                System.out.println("Create a account:\n");
                System.out.print("NameAccount: ");
                input(socket);
                checkOutPut(receiveRead, socket);
            }
            default -> {
                System.out.println("Error: unachievable");
            }
        }
    }

    private void input(Socket socket) throws IOException {
        String accountName = null;
        String accountPassword = null;
        try {
            accountName = sendAccount();
        } catch (IOException e) {
            System.out.println("error send account name message");
            e.printStackTrace();
        }
        System.out.print("Password: ");
        try {
            accountPassword = sendAccount();
        } catch (IOException e) {
            System.out.println("error send account password message");
            e.printStackTrace();
        }
        String accountMessage = createAccountMessage(accountName,accountPassword);
        sendAccountMessage(accountMessage, socket);
    }

    private void checkOutPut(BufferedReader receiveRead, Socket socket) throws IOException {
        String receiveMessage;
        receiveMessage = String.valueOf(receiveRead.readLine());
        receiveMessage = removeNonAscii(receiveMessage);
        receiveMessage = replaceUnreadable(receiveMessage);
        if(!Objects.equals(receiveMessage, "0")) //receive from server
        {
            System.out.print("from server: ");
            System.out.println(receiveMessage); // displaying at DOS prompt
            if(receiveMessage.equals("Login successfully: Welcome to PC COVID")){
                System.out.println("trang chu");
            }else if(receiveMessage.equals("Create a account successfully: Please enter personal information")){
                String idUser = String.valueOf(receiveRead.readLine());
                idUser = removeNonAscii(idUser);
                idUser = replaceUnreadable(idUser);
                createInforAccount(idUser,socket);
            }
        }
    }

    private void createInforAccount(String idUser,Socket socket) throws IOException {
        account userAccount = new account();
        userAccount.setIdUser(idUser);
        System.out.print("First name: ");
        String firstName = sendAccount();
        userAccount.setFirstName(firstName);
        System.out.print("Last name: ");
        String lastName = sendAccount();
        userAccount.setFirstName(lastName);
        System.out.print("Your cardId: ");
        String cardId = sendAccount();
        userAccount.setFirstName(cardId);
        System.out.print("Birthday (dd/mm/yyyy): ");
        String birthday = sendAccount();
        userAccount.setFirstName(birthday);
        System.out.print("Gender: ");
        String gender = sendAccount();
        userAccount.setFirstName(gender);
        System.out.print("Number phone: ");
        String numberPhone = sendAccount();
        userAccount.setFirstName(numberPhone);
        System.out.print("Email: ");
        String email = sendAccount();
        userAccount.setFirstName(email);
        System.out.print("Address: ");
        String address = sendAccount();
        userAccount.setFirstName(address);
        userAccount.setFirstName("normal");
        String message = createInforAccountMessage(idUser,firstName,lastName,cardId,birthday,gender,numberPhone,email,address,"normal");
        sendAccountMessage(message,socket);
//        System.out.println(message);
    }

    private String createInforAccountMessage(String idUser, String firstName, String lastName, String cardId, String birthday, String gender, String numberPhone,String email, String address, String state){
        return idUser + "_" + firstName + "_" + lastName + "_" + cardId + "_" + birthday + "_" + gender + "_" + numberPhone + "_" +email + "_" + address + "_" + state;
    }

    private String sendAccount() throws IOException {
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

    public static void main(String[] args) throws Exception {
        Socket sock = new Socket("127.0.0.1", 9999);
        ClientJava clientJava = new ClientJava();
        clientJava.menuChoice(sock);
    }
}