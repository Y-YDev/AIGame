public class Board {
    public int[] cells = new int[24];//Cases du jeu
    public int totalSeed = 0;

    public boolean P1Turn = true;//true = Tour du player 1

    boolean isReduce=false;
    //Score des joueurs
    public int scoreP1 = 0;
    public int scoreP2 = 0;

    public Board(){ }

    public Board(Board board){
        cells = new int[board.cells.length];
        for(int i = 0;i<board.cells.length;i++){
            cells[i] = board.cells[i];
        }
        this.totalSeed = board.totalSeed;
        this.P1Turn = board.P1Turn;
        this.scoreP1 = board.scoreP1;
        this.scoreP2 = board.scoreP2;
        this.isReduce = board.isReduce;
    }


    public boolean endPosition(){

        if(nBSeedsOnCells(P1Turn) == 0){//Starving rule
            return true;
        }

        if(totalSeed < 8){//Teacher rule
            return true;
        }

        return false;
    }

    public boolean correctMove(int move){

        if((move%2==0) != P1Turn) return false;//Joue sur la case de l'autre.

        if(cells[move] == 0) return false;//Il joue sur une case vide.

        return true;
    }

    public void playMove(int move){
        int initialIdx = move;
        int seeds = cells[move];
        cells[move] = 0;

        while(seeds != 0){//On sème
            move = getIndex(move+1);

            if(move != initialIdx){ //Loop rule
                cells[move]++;
                seeds--;
            }
        }

        while(cells[move] == 2 || cells[move] == 3){ //Prise
            if(P1Turn){
                scoreP1 += cells[move];
            }else{
                scoreP2 += cells[move];
            }
            totalSeed -= cells[move];
            cells[move] = 0;
            move = getIndex(move-1);
        }

        if(nBSeedsOnCells(!P1Turn) == 0){//Starving end
            if(P1Turn){//P1 take P2 starve
                scoreP1 += totalSeed;
            }
            else{
                scoreP2 += totalSeed;
            }
        }
        
        if(!isReduce && totalSeed <= 48){//Teacher rule
            changeBoard();
        }
    }

    protected int getIndex(int idx){
        idx %= cells.length;

        if(idx < 0){
            return cells.length+idx;
        }
        else{
            return idx;
        }
    }

    protected int nBSeedsOnCells(boolean P1){
        int seeds = 0;

        for(int i =0; i < cells.length; i++){
            if((i%2==0) == P1) {
                seeds += cells[i];
            }
        }

        return seeds;
    }

    protected void changeBoard(){
        int[] newCells = new int[12];
        isReduce = true;
        for(int i =0;i<12;i++){//Merge des cases (Teacher rule)
            for(int j = i*2; j<=i*2+1;j++){
                newCells[i] += cells[j];
            }
        }
        cells = newCells;
    }

    protected void printBoard(){
        for(int i = 0;i<cells.length/2;i++) {
        	if(i%2==0) System.out.print("#");
            System.out.print(cells[i] + " | ");
        }
        System.out.println();

        for(int i = cells.length-1;i>=cells.length/2;i--) {
        	if(i%2==0) System.out.print("#");
            System.out.print(cells[i] + " | ");
        }
        System.out.println();
    }
}
