package inforUser;

import java.util.Objects;

public class account {
    private String userAccount;
    private String idUser;
    private String firstName;
    private String lastname;
    private String cardId;
    private String birthOfDay;
    private String gender;
    private String numberPhone;
    private String email;
    private String address;
    private String state;

    public account() {
    }

    public account(String userAccount, String idUser, String firstName, String lastname, String cardId, String birthOfDay, String gender, String numberPhone, String email,String address, String state) {
        this.userAccount = userAccount;
        this.idUser = idUser;
        this.firstName = firstName;
        this.lastname = lastname;
        this.cardId = cardId;
        this.birthOfDay = birthOfDay;
        this.gender = gender;
        this.numberPhone = numberPhone;
        this.email = email;
        this.address = address;
        this.state = state;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getUserAccount() {
        return userAccount;
    }

    public void setUserAccount(String userAccount) {
        this.userAccount = userAccount;
    }

    public String getIdUser() {
        return idUser;
    }

    public void setIdUser(String idUser) {
        this.idUser = idUser;
    }

    public String getFirstName() {
        return firstName;
    }

    public void setFirstName(String firstName) {
        this.firstName = firstName;
    }

    public String getLastname() {
        return lastname;
    }

    public void setLastname(String lastname) {
        this.lastname = lastname;
    }

    public String getCardId() {
        return cardId;
    }

    public void setCardId(String cardId) {
        this.cardId = cardId;
    }

    public String getBirthOfDay() {
        return birthOfDay;
    }

    public void setBirthOfDay(String birthOfDay) {
        this.birthOfDay = birthOfDay;
    }

    public String getGender() {
        return gender;
    }

    public void setGender(String gender) {
        this.gender = gender;
    }

    public String getNumberPhone() {
        return numberPhone;
    }

    public void setNumberPhone(String numberPhone) {
        this.numberPhone = numberPhone;
    }

    public String getAddress() {
        return address;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    public String getState() {
        return state;
    }

    public void setState(String state) {
        this.state = state;
    }
}
