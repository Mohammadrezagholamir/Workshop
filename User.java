package models;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import models.enums.*;

public class User {
    private String Username;
    private String Password;
    private String Email;
    private String name;
    private ArrayList<Group> MyGroups = new ArrayList<>();
    private ArrayList<Expense> myUserExpence = new ArrayList<>();
    private HashMap<User, Integer> debts = new HashMap<>();
    private Currency myCurrency = Currency.GTC;

    public User(String Username, String Password, String Email, String name, Currency currency) {
        this.Password = Password;
        this.Username = Username;
        this.Email = Email;
        this.name = name;
        this.myCurrency = currency;
    }

    public String getEmail() {
        return Email;
    }

    public String getPassword() {
        return Password;
    }

    public String getUsername() {
        return Username;
    }

    public String getName() {
        return name;
    }

    public ArrayList<Group> getGroup() {
        return MyGroups;
    }

    public ArrayList<Expense> getExpense() {
        return myUserExpence;
    }

    public Currency getCurrency() {
        return myCurrency;
    }

    public void addExpense(Expense expense, Currency currency) {
        myUserExpence.add(expense);
        User talabkar = expense.getTalabkar();
        User bedehkar = expense.getBedehkar();
        int valueInGTC = Currency.convert(expense.getValue(), currency, Currency.GTC);

        if (!talabkar.equals(bedehkar)) {
            talabkar.addDebt(bedehkar, valueInGTC); // Creditor owes debtor
            bedehkar.addDebt(talabkar, -valueInGTC); // Debtor owes creditor

            // Simplify debts
            talabkar.simplifyDebts(talabkar, bedehkar);
            bedehkar.simplifyDebts(talabkar, bedehkar);
        }
    }

    public void addDebt(User debtor, int amount) {
        debts.put(debtor, debts.getOrDefault(debtor, 0) + amount);
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
        // تبدیل مقدار به ارز پایه (GTC)
        int remainingAmount = Currency.convert(amount, App.getLoggedIn().getCurrency(), Currency.GTC);
    
        // بررسی بدهی‌ها
        int myDebtToUser = this.debts.getOrDefault(user, 0); // من چقدر به کاربر بدهکارم
        int userDebtToMe = user.debts.getOrDefault(this, 0); // کاربر چقدر به من بدهکار است
    
        // اگر کاربر به من بدهکار باشد
        if (userDebtToMe > 0) {
            if (remainingAmount >= userDebtToMe) {
                // تسویه کامل بدهی کاربر به من
                user.debts.remove(this); // بدهی کاربر به من حذف می‌شود
                remainingAmount -= userDebtToMe; // باقیمانده مقدار تسویه
            } else {
                // تسویه جزئی بدهی کاربر به من
                user.debts.put(this, userDebtToMe - remainingAmount); // بدهی کاربر به من کاهش می‌یابد
                remainingAmount = 0; // باقیمانده مقدار تسویه صفر می‌شود
            }
        }
    
        // اگر من به کاربر بدهکار باشم
        if (myDebtToUser > 0) {
            if (remainingAmount >= myDebtToUser) {
                // تسویه کامل بدهی من به کاربر
                this.debts.remove(user); // بدهی من به کاربر حذف می‌شود
                remainingAmount -= myDebtToUser; // باقیمانده مقدار تسویه
            } else {
                // تسویه جزئی بدهی من به کاربر
                this.debts.put(user, myDebtToUser - remainingAmount); // بدهی من به کاربر کاهش می‌یابد
                remainingAmount = 0; // باقیمانده مقدار تسویه صفر می‌شود
            }
        }
    
        // ساده‌سازی بدهی‌ها پس از تسویه
        this.simplifyDebts(this, user);
    
        // بررسی بدهی نهایی
        int finalDebt = this.debts.getOrDefault(user, 0);
        if (finalDebt == 0) {
            return "You are settled with " + user.getUsername() + "!";
        } else if (finalDebt > 0) {
            return user.getUsername() + " owes you " + Currency.convert(finalDebt, Currency.GTC, this.myCurrency) + " " + this.myCurrency.name() + "!";
        } else {
            return "You owe " + user.getUsername() + " " + Currency.convert(-finalDebt, Currency.GTC, this.myCurrency) + " " + this.myCurrency.name() + "!";
        }
    }

    public String showBalance(User otherUser) {
        String balanceMessage = null;
        int myDebtGTC = debts.getOrDefault(otherUser, 0);
        int theirDebtGTC = otherUser.debts.getOrDefault(this, 0);

        int myDebt = Currency.convert(myDebtGTC, Currency.GTC, this.myCurrency);
        int theirDebt = Currency.convert(theirDebtGTC, Currency.GTC, this.myCurrency);

        if (myDebt > 0) {
            balanceMessage = "You owe " + otherUser.getUsername() + " " + myDebt + " " + this.myCurrency.name() + ".";
        } else if (theirDebt > 0) {
            balanceMessage = otherUser.getUsername() + " owes you " + theirDebt + " " + this.myCurrency.name() + ".";
        } else {
            return "You are settled with " + otherUser.getUsername() + ".";
        }

        ArrayList<String> groupNames = new ArrayList<>();
        for (Group g : otherUser.MyGroups) {
            for (Expense e : myUserExpence) {
                if ((e.getBedehkar().equals(this) && e.getTalabkar().equals(otherUser)) || (e.getBedehkar().equals(otherUser) && e.getTalabkar().equals(this))) {
                    groupNames.add(g.getName());
                    break;
                }
            }
        }
        return balanceMessage + " In groups: " + String.join(", ", groupNames) + "!";
    }

    public void addToGroup(Group temp) {
        MyGroups.add(temp);
    }

    public void showMyGroups() {
        for (Group group : MyGroups) {
            System.out.println(group.toString());
            group.showMembers();
            System.out.println("--------------------");
        }
    }
}