package program;

import java.util.Scanner;

public class Player implements IPlayer{

    private boolean isP1 = false;

    public Player(boolean isP1){
        this.isP1 = isP1;
    }

    @Override
    public int play(Board board){

        Scanner sc = new Scanner(System.in);

        int answer = sc.nextInt();
        return answer-1;//Case 1 d'index 0
    }
}
