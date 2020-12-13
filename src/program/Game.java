package program;

public class Game{
    Board board;

    public Game(){
        board = new Board();
        initBoard();
    }
    //best p1 -> 8 de prof
    private IPlayer P1 = new AI3(true,8);//Cases impaires
    private IPlayer P2 = new AI4(false,8);//Cases paires

    protected void initBoard(){
        board.totalSeed = 4*24;//96 graines

        for(int i =0; i<24;i++){//Start avec 4 graines
            board.cells[i] = 4;
        }

        board.scoreP2 = 0;
        board.scoreP1 = 0;
        board.P1Turn = true;
    }

    public void gameLoop() throws Exception {
    	
    	board.printBoard();
        while(!board.endPosition()){

        	
            int move;
            if(board.P1Turn){
                move = P1.play(board);
                System.out.println("P1 a joué : "+(move+1));
            }
            else{
                move = P2.play(board);
                System.out.println("P2 a joué : "+(move+1));
            }

            if(board.correctMove(move)){
                board.playMove(move);
                board.printBoard();//Print
            }
            else{//INCORRECT MOVE
                throw new Exception("INVALID MOVE");
            }
            System.out.println("SCORE P1 = "+board.scoreP1+" | SCORE P2 = "+board.scoreP2+"\n");

            board.P1Turn = !board.P1Turn;//Changement de joueur.
        }

        System.out.println("SCORE P1 = "+board.scoreP1+" | SCORE P2 = "+board.scoreP2);
    }
}
