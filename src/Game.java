public class Game{
    Board board;

    public Game(){
        board = new Board();
        initBoard();
    }
    private IPlayer P1 = new AI(true);//Cases impaires
    private IPlayer P2 = new AI(false);//Cases paires

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

        while(!board.endPosition()){

            board.printBoard();//Print

            int move;
            if(board.P1Turn){
                move = P1.play(board);
            }
            else{
                move = P2.play(board);
            }

            if(board.correctMove(move)){
                board.playMove(move);
            }
            else{//INCORRECT MOVE
                throw new Exception("INVALID MOVE");
            }

            if(board.totalSeed <= 48){//Teacher rule
                board.changeBoard();
            }

            board.P1Turn = !board.P1Turn;//Changement de joueur.
        }

        System.out.println("SCORE P1 = "+board.scoreP1+" | SCORE P2 = "+board.scoreP2);
    }
}
