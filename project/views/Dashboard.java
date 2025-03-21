package views;
import java.util.regex.*;

import models.App;
import models.enums.*;

import java.util.ArrayList;
import java.util.Scanner;
import controllers.*;

/*
Explanation:
- This is a view class for the dashboard.
- This class should use to check inputs and print outputs for the dashboard.
- notice that : this class should not have any logic and just use it to get inputs and handle it to use correct methods in controller.
 */

public class Dashboard implements AppMenu {
    final DashboardController controller = new DashboardController();
    public void check(Scanner scanner){
        String input = scanner.nextLine();
        input = input.trim();
        Matcher matcher ;
        Matcher match;
        if(models.enums.DashboardCommands.DASHBORD.getMatcher(input) != null){
            
            
            input = input.replaceAll("\\s+", " ").trim();
            matcher = models.enums.DashboardCommands.DASHBORD.getMatcher(input);
            String nameOfGroup = matcher.group(1);
            String testType = matcher.group(2);
            System.out.println(controller.createGroup(nameOfGroup, testType));
        }
        else if(models.enums.DashboardCommands.SHOWMYGROUPS.getMatcher(input)!=null){
            matcher = models.enums.DashboardCommands.SHOWMYGROUPS.getMatcher(input);
            controller.ShowGroups(App.getLoggedIn());
            
        }
        else if(models.enums.DashboardCommands.ADDTOGROUP.getMatcher(input)!=null){
            input = input.replaceAll("\\s+", " ").trim();
            matcher = models.enums.DashboardCommands.ADDTOGROUP.getMatcher(input);
            String Username = matcher.group(1);
            String Email = matcher.group(2);
            int groupIdForAddUser = Integer.parseInt(matcher.group(3));
            System.out.println(controller.AddToGroup(Username, Email, groupIdForAddUser));   
        }
        else if(models.enums.DashboardCommands.ADDEXPENCE.getMatcher(input)!=null){
            input = input.replaceAll("\\s+", " ").trim();
            matcher = models.enums.DashboardCommands.ADDEXPENCE.getMatcher(input);
            int groupIdForAddExpense =Integer.parseInt(matcher.group(1)) ;
            String typeOfEq = matcher.group(2);
            int totalExpense = Integer.parseInt(matcher.group(3));
            int numberOfUsers = Integer.parseInt(matcher.group(4));
            if(typeOfEq.equals("equally")){
                
                ArrayList<String> usernamesForExpense = new ArrayList<>();
                for(int i=0; i<numberOfUsers ; i++){
                    String Usernames = scanner.nextLine();
                    usernamesForExpense.add(Usernames);
                }
                System.out.println(controller.addExpenseEqually(groupIdForAddExpense, totalExpense, numberOfUsers, usernamesForExpense, matcher.group(3)));
            }
            else if(typeOfEq.equals("unequally")){
                
                ArrayList<String> usernameForExpense = new ArrayList<>();
                ArrayList<Integer> valueForExpense = new ArrayList<>();
                for(int i=0 ; i<numberOfUsers ; i++){
                    String usernames = scanner.nextLine();
                    usernames = usernames.trim();
                    if(models.enums.DashboardCommands.UNEQUALLY.getMatcher(usernames)!=null){
                        usernames = usernames.replaceAll("\\s+", " ").trim();
                        match = models.enums.DashboardCommands.UNEQUALLY.getMatcher(usernames);
                        usernameForExpense.add(match.group(1));
                        valueForExpense.add(Integer.parseInt(match.group(2)));
                    }
                    else {
                        System.out.println("invalid command!");
                        break;
                    }
                }
                System.out.println(controller.addExpenseUnequally(groupIdForAddExpense, totalExpense, usernameForExpense, valueForExpense, matcher.group(3)));

            }

        }
        else if(models.enums.DashboardCommands.SHOWBALANCE.getMatcher(input)!=null){
            input = input.replaceAll("\\s+", " ").trim();
            matcher = models.enums.DashboardCommands.SHOWBALANCE.getMatcher(input);
            String username = matcher.group(1);
            System.out.println(controller.showBalance(username));
        }
        else if(models.enums.DashboardCommands.SETTLEUP.getMatcher(input)!=null){
            input = input.replaceAll("\\s+", " ").trim();
            matcher = models.enums.DashboardCommands.SETTLEUP.getMatcher(input);
            String username = matcher.group(1);
            int inputMoney = Integer.parseInt(matcher.group(2));
            System.out.println(controller.settleUp(username, inputMoney, matcher.group(2)));
        }
        else if(models.enums.DashboardCommands.GOTOPROFILE.getMatcher(input)!=null){
            System.out.println(controller.goToProfile());
        }
        else if(models.enums.DashboardCommands.LOGOUT.getMatcher(input)!=null){
            System.out.println(controller.logout());
        }
        else if(models.enums.SignUpMenuCommands.EXIT.getMatcher(input)!=null){
            App.setMenu(Menu.ExitMenu);
        }
        else {
            System.out.println("invalid command!");
        }
    }

}
