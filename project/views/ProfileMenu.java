package views;
import java.util.Scanner;
import controllers.*;
import java.util.regex.*;

import models.App;
import models.enums.*;
/*
Explanation: 
- This is a view class for profile menu.
- This class should use to check inputs and print outputs for profile menu.
- notice that : this class should not have any logic and just use it to get inputs and handle it to use correct methods in controller.
 */


public class ProfileMenu implements AppMenu {
    private final ProfileMenuController controller = new ProfileMenuController();
    public void check(Scanner scanner){
        String input = scanner.nextLine();
        Matcher matcher ;
        if(models.enums.ProfileMenuCommands.SHOWPROFILE.getMatcher(input)!=null){
            System.out.println(controller.userInfo(App.getLoggedIn()));
        }
        else if(models.enums.ProfileMenuCommands.LOGOUT.getMatcher(input)!=null){
            System.out.println(controller.logout());
        }
        else if(models.enums.ProfileMenuCommands.CHANGECURRENCY.getMatcher(input)!=null){
            matcher= models.enums.ProfileMenuCommands.CHANGECURRENCY.getMatcher(input);
            String newCurrency =  matcher.group(1);
            System.out.println(controller.changeCurrency(newCurrency));
        }
        else if(models.enums.ProfileMenuCommands.CHANGENAME.getMatcher(input)!=null){
            matcher = models.enums.ProfileMenuCommands.CHANGENAME.getMatcher(input);
            String name = matcher.group(1);
            System.out.println(controller.changeName(name));
        }
        else if(models.enums.ProfileMenuCommands.CHANGEPASSWORD.getMatcher(input)!=null){
            matcher = models.enums.ProfileMenuCommands.CHANGEPASSWORD.getMatcher(input);
            String oldPassword = matcher.group(1);
            String newPassword = matcher.group(2);
            System.out.println(controller.changePassword(oldPassword, newPassword));
        }
        else if(models.enums.ProfileMenuCommands.BACK.getMatcher(input)!=null){
            App.setMenu(Menu.Dashboard);
            System.out.println("you are now in dashboard!");
        }
        else if(models.enums.SignUpMenuCommands.EXIT.getMatcher(input)!=null){
            App.setMenu(Menu.ExitMenu);
        }
        else {
            System.out.println("invalid command!");
        }
    }
}
