package models;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import models.enums.*;
import javax.jws.soap.SOAPBinding.Use;

import javafx.util.converter.CurrencyStringConverter;

/*
Explanation:
- User is definitely an object in our app.
- put the information that you need to store about the user here.
- you can put some functions here to manage the user data too.
 */

public class User {
    private String Username;
    private String Password;
    private String Email;
    private String name;
    private ArrayList<Group> MyGroups = new ArrayList<>();
    private ArrayList<Expense> myUserExpence = new ArrayList<>();
    private HashMap<User , Integer> debts = new HashMap<>();
    private Currency myCurrency = Currency.GTC;
    public User(String Username , String Password , String Email , String name , Currency currency){
        this.Password = Password;
        this.Username = Username;
        this.Email = Email;
        this.name = name;
        this.myCurrency = currency;
    }
    User getMySelf(){
        return this;
    }
    public String getEmail(){
        return Email;
    }
    public String getPassword(){
        return Password;
    }
    public String getUsername(){
        return Username;
    }
    public String getName(){
        return name;
    }
    public ArrayList<Group> getGroup(){
        return MyGroups;
    }
    public ArrayList<Expense> getExpense(){
        return myUserExpence;
    }
    public Currency getCurrency(){
        return myCurrency;
    }
    public void setCurrency(Currency c){
        this.myCurrency = c;
    }
    public void setName(String name){
        this.name =name;
    }
    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (obj == null || getClass() != obj.getClass()) return false;
        User user = (User) obj;
        return Username.equals(user.Username);
    }

    @Override
    public int hashCode() {
        return Username.hashCode();
    }

    public void addExpense(Expense expense, Currency currency , int numberOfusers) {
        myUserExpence.add(expense);
        User talabkar = expense.getTalabkar();
        User bedehkar = expense.getBedehkar();
        int valueInGTC = Currency.convert(expense.getValue(), currency, Currency.GTC);
        
        if (!talabkar.equals(bedehkar)) {
            
            debts.put(talabkar, debts.getOrDefault(talabkar, 0) + valueInGTC );
            bedehkar.debts.put(this, bedehkar.debts.getOrDefault(this, 0) + valueInGTC  );
            simplifyDebts(talabkar, bedehkar);
            
        }

    }

    public void simplifyDebts(User user1, User user2) {
        int debt1to2 = user1.debts.getOrDefault(user2, 0);
        int debt2to1 = user2.debts.getOrDefault(user1, 0);
        int finalDebt = debt1to2 - debt2to1;
        
        if (finalDebt > 0) {
            user1.debts.put(user2, finalDebt);
            user2.debts.remove(user1);
        } else if (finalDebt < 0) {
            user2.debts.put(user1, -finalDebt);
            user1.debts.remove(user2);
        } else {
            user1.debts.remove(user2);
            user2.debts.remove(user1);
        }
    }

    public String settleUpUser(User user, int amount) {
        simplifyDebts(this, user);

        if (this.debts.containsKey(user)) {
            int sum = this.debts.get(user);
            if(amount > sum ){
                this.debts.remove(user);
                user.debts.put(this, amount - sum);
                return user.getUsername() + " owes you " + Currency.convert(amount - sum , Currency.GTC, myCurrency) + " " +myCurrency+ " now!" ;
            }
            else if (amount == sum) {
                this.debts.remove(user);
                return "You are settled with " + user.getUsername() + " now!";
            } else {
                this.debts.put(user, sum - amount);
                return "You owe " + user.getUsername() + " " + Currency.convert(amount - sum , Currency.GTC, myCurrency) + " " + this.myCurrency + " now!";
            }
        } else if (user.debts.containsKey(this)) {
            int sum = user.debts.get(this);
            user.debts.put(this , sum + amount);
            return user.getUsername() + " owes you " + Currency.convert(amount + sum , Currency.GTC, myCurrency) + " " + myCurrency + " now!";
        }
        return "No debts exist between you and " + user.getUsername();
    }

    public String showBalance(User user) {
        ArrayList<String> moshtarakGroups = new ArrayList<>();
        for(Group g : App.groups){
            if(g.existsInMe(user)&& g.existsInMe(this)){
                
                moshtarakGroups.add(g.getName());
            }
        }
        if (this.debts.containsKey(user)) {
            int sum = this.debts.get(user);
            return "You owe " + user.getUsername() + " " + Currency.convert(sum, Currency.GTC, myCurrency) + " " + this.myCurrency   +" " + "in" + " " + String.join(", ", moshtarakGroups) + "!";
        } else if (user.debts.containsKey(this)) {
            int sum = user.debts.get(this);
            return user.getUsername() + " owes you " + Currency.convert(sum, Currency.GTC, myCurrency) + " " + this.myCurrency +" " + "in" + " " + String.join(", ", moshtarakGroups) + "!";
        }
        return "You are settled with " + user.getUsername();
    }


    public void addToGroup(Group temp){
        MyGroups.add(temp);
    }
    public void showMyGroups(){
        for( Group group : MyGroups){
            //System.out.println("khara");
            System.out.println(group.toString() );
            group.showMembers();
            System.out.println("--------------------");
        }
    }
}