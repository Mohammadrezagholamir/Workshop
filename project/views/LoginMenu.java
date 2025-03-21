package views;
import java.util.Scanner;
import java.util.*;
import controllers.*;
import models.App;
import models.enums.Menu;

/*
Explanation:
- This is a view class for the login menu.
- This class should use to check inputs and print outputs for the login menu.
- notice that : this class should not have any logic and just use it to get inputs and handle it to use correct methods in controller.
 */
import java.util.regex.Matcher;


public class LoginMenu implements AppMenu{
    final LoginMenuController controller = new LoginMenuController();
    public void check(Scanner scanner){
        String input = scanner.nextLine();
        input = input.trim();
        Matcher matcher;
        if(models.enums.LoginMenuCommands.LOGIN.getMatcher(input)!= null){
            input = input.replaceAll("\\s+", " ").trim();
            matcher = models.enums.LoginMenuCommands.LOGIN.getMatcher(input);
            String UsernameForLogin = matcher.group(1);
            String passwordForLogin = matcher.group(2);
            System.out.println(controller.login(UsernameForLogin, passwordForLogin , App.users));

        }
        else if(models.enums.LoginMenuCommands.GOTOSIGNUP.getMatcher(input)!=null){
            matcher = models.enums.LoginMenuCommands.GOTOSIGNUP.getMatcher(input);
            System.out.println(controller.goToSignUp());
        }
        else if(models.enums.LoginMenuCommands.GOTOLOGIN.getMatcher(input)!= null){
            System.out.println(controller.goToLogin());
        }
        else if(models.enums.SignUpMenuCommands.EXIT.getMatcher(input)!=null){
            App.setMenu(Menu.ExitMenu);
        }
        else if(models.enums.LoginMenuCommands.FORGOTPASSWORD.getMatcher(input)!=null){
            input = input.replaceAll("\\s+", " ").trim();
            matcher = models.enums.LoginMenuCommands.FORGOTPASSWORD.getMatcher(input);
            
            String username = matcher.group(1);
            String email = matcher.group(2);
            System.out.println(controller.ForotPassword(email, username));
        }
        else {
            System.out.println("invalid command!");
        }

    }
}
