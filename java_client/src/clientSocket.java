import inforUser.account;
import java.time.LocalDateTime; // import the LocalDateTime class
import java.io.*;
import java.net.*;
import java.util.Objects;
import java.util.Scanner;


class ClientJava {
    private void menuChoice(Socket socket, account user) throws IOException {
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
                checkOutPut(receiveRead, socket, user);
            }
            case 2 -> {
                sendAccountMessage(String.valueOf(choice),socket);
                InputStream istream = socket.getInputStream();
                BufferedReader receiveRead = new BufferedReader(new InputStreamReader(istream), 1024);
                System.out.println("Create a account:\n");
                System.out.print("NameAccount: ");
                input(socket);
                checkOutPut(receiveRead, socket, user);
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


    private void checkOutPut(BufferedReader receiveRead, Socket socket, account user) throws IOException {
        String receiveMessage;
        receiveMessage = String.valueOf(receiveRead.readLine());
        receiveMessage = removeNonAscii(receiveMessage);
        receiveMessage = replaceUnreadable(receiveMessage);
        if(!Objects.equals(receiveMessage, "0")) //receive from server
        {
            System.out.print("from server: ");
            System.out.println(receiveMessage); // displaying at DOS prompt
            if(receiveMessage.equals("Login successfully: Welcome to PC COVID")){
                System.out.println("\n--HOME PAGE--");
                homePage(receiveRead,socket, user);
            }else if(receiveMessage.equals("Create a account successfully: Please enter personal information")){
                String idUser = String.valueOf(receiveRead.readLine());
                idUser = removeNonAscii(idUser);
                idUser = replaceUnreadable(idUser);
                createInforAccount(idUser,socket);
                String receiveMessage2;
                receiveMessage2 = String.valueOf(receiveRead.readLine());
                receiveMessage2 = removeNonAscii(receiveMessage2);
                receiveMessage2 = replaceUnreadable(receiveMessage2);
                if(!Objects.equals(receiveMessage2, "0")) //receive from server
                {
                    System.out.print("from server: ");
                    System.out.println(receiveMessage2); // displaying at DOS prompt
                    if(receiveMessage2.equals("Create information successfully -> Let login to PC COVID")){
                        System.out.println("Let't go to PC COVID");
                    }else{
                        System.out.println("Please create a new account");
                    }
                    ClientJava clientJava = new ClientJava();
                    clientJava.menuChoice(socket, user);
                }
            }
        }
    }

    private void homePage(BufferedReader receiveRead,Socket socket, account user) throws IOException {
//        InputStream istream = socket.getInputStream();
//        BufferedReader receiveRead = new BufferedReader(new InputStreamReader(istream), 1024);
        menuHomePage(receiveRead,socket, user);
    }

    private void menuHomePage(BufferedReader receiveRead,Socket socket, account user) throws IOException {
        int choice;
        Scanner scanner = new Scanner(System.in);
        System.out.println("-----------------------PC COVID-----------------------");
        System.out.println("1: Xem thong tin ca nhan cua ban");
        System.out.println("2: Khai bao y te");
        System.out.println("3: Lich su di chuyen");
        System.out.println("4: Chinh sua thong tin ca nhan");
        System.out.println("5: Exit");
        System.out.print("Your choice: ");
        choice = scanner.nextInt();
        switch (choice){
            case 1 ->{
                sendAccountMessage(String.valueOf(choice),socket);
                String receiveMessage2 = "";
                receiveMessage2 = String.valueOf(receiveRead.readLine());
                receiveMessage2 = removeNonAscii(receiveMessage2);
                receiveMessage2 = replaceUnreadable(receiveMessage2);
                if(!Objects.equals(receiveMessage2, "0")){
                    inputTheUser(receiveMessage2, user);
                }else if(receiveMessage2.equals("")){
                    System.out.println("Can't read your information!");
                }
                showYourInformation(user);
                menuHomePage(receiveRead,socket,user);
            }
            case 2 ->{
                sendAccountMessage(String.valueOf(choice),socket);
                System.out.println("Nhap ma dia diem:");
                String codeOfLocation = sendAccount();
                //TODO

            }
            case 3 ->{
                System.out.println("Error: unachievable3");
            }
            case 4 ->{
                sendAccountMessage(String.valueOf(choice),socket);
                String idUser = user.getIdUser();
                createInforAccount(idUser,socket);
                String receiveMessage2;
                receiveMessage2 = String.valueOf(receiveRead.readLine());
                receiveMessage2 = removeNonAscii(receiveMessage2);
                receiveMessage2 = replaceUnreadable(receiveMessage2);
                if(!Objects.equals(receiveMessage2, "0")) //receive from server
                {
                    System.out.print("from server: ");
                    System.out.println(receiveMessage2); // displaying at DOS prompt
                }
                menuHomePage(receiveRead,socket,user);
            }
            case 5 ->{
                sendAccountMessage(String.valueOf(choice),socket);
                ClientJava clientJava = new ClientJava();
                clientJava.menuChoice(socket, user);
            }
            case 6 ->{
                System.out.println("Error: unachievable1");
            }
        }
    }

    private void showYourInformation(account user){
        showInfor(user);
    }

    private void showInfor(account user) {
        System.out.println("--------- Your information ---------");
        System.out.println("ID: " + user.getIdUser());
        System.out.println("Your name: " + user.getLastname() + " " + user.getFirstName());
        System.out.println("Your cardId: " + user.getCardId());
        System.out.println("Birthday (dd/mm/yyyy): " + user.getBirthOfDay());
        System.out.println("Gender: " + user.getGender());
        System.out.println("Number phone: " + user.getNumberPhone());
        System.out.println("Email: " + user.getEmail());
        System.out.println("Address: "+ user.getAddress());
        System.out.println("State: " + user.getState());
    }


    private void inputTheUser(String receiveMessage, account user) {
        String[] listInfor = receiveMessage.split("_");
        user.setIdUser(listInfor[0]);
        user.setFirstName(listInfor[1]);
        user.setLastname(listInfor[2]);
        user.setCardId(listInfor[3]);
        user.setBirthOfDay(listInfor[4]);
        user.setGender(listInfor[5]);
        user.setNumberPhone(listInfor[6]);
        user.setEmail(listInfor[7]);
        user.setAddress(listInfor[8]);
        user.setState(listInfor[9]);
    }

    private void createInforAccount(String idUser,Socket socket) throws IOException {
        account userAccount = new account();
        userAccount.setIdUser(idUser);
        System.out.print("First name: ");
        String firstName = sendAccount();
        userAccount.setFirstName(firstName);
        System.out.print("Last name: ");
        String lastName = sendAccount();
        userAccount.setLastname(lastName);
        System.out.print("Your cardId: ");
        String cardId = sendAccount();
        userAccount.setCardId(cardId);
        System.out.print("Birthday (dd/mm/yyyy): ");
        String birthday = sendAccount();
        userAccount.setBirthOfDay(birthday);
        System.out.print("Gender: ");
        String gender = sendAccount();
        userAccount.setGender(gender);
        System.out.print("Number phone: ");
        String numberPhone = sendAccount();
        userAccount.setNumberPhone(numberPhone);
        System.out.print("Email: ");
        String email = sendAccount();
        userAccount.setEmail(email);
        System.out.print("Address: ");
        String address = sendAccount();
        userAccount.setAddress(address);
        userAccount.setState("normal");
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
        return name+ "_" +password;
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
        account user = new account();
        Socket sock = new Socket("127.0.0.1", 9999);
        ClientJava clientJava = new ClientJava();
        clientJava.menuChoice(sock, user);
    }
}