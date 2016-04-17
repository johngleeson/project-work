
import java.util.Scanner;

public class ShiftCipherMain {

    public static void main(String args[]) {
        int key = 0;

        Scanner inputScanner = new Scanner(System.in);
        System.out.printf("This is a simple shift cipher program.\n");
        System.out.printf("Please enter the message you want to encode:\n");
        String input = inputScanner.nextLine();
        do {
            System.out.printf("Enter a shift key between 1 and 26:\n");
            key = inputScanner.nextInt();
        } while (key < 1 || key > 26);

        for (int i = 0; i < input.length(); i++) {
            char character = input.charAt(i);
            if ((character >= 'a') && (character <= 'z')) {
                character = (char) (character + key);
                if (character > 'z') {
                    character = (char) (character - 26);
                }
            } else {
                System.out.printf("");
            }
            System.out.printf("%c", character);

        }
        System.out.println();
    }
}
