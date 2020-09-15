import java.util.Scanner;
public class SAOJ2 {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        long N = sc.nextLong();
        System.out.println("#include <stdio.h>");
        System.out.println("int main() {");
        System.out.println("    printf(\"" + ((N*N+N)/2) + "\");");
        System.out.println("    return 0;");
        System.out.println("}");
    }
}
