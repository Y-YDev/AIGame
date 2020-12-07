package program;

public class AI implements IPlayer {

    private boolean isP1 = false;
    public final static int VALMAX=96;
    public int maxDepth = 7;
    

    public AI(boolean isP1,int maxDepth){
        this.isP1 = isP1;
        this.maxDepth = maxDepth;
    }


    @Override
    public int play(Board board){
        int maxResult= 0;
        int index = -1;
        for(int i = 0; i<board.cells.length/2;i++){
            Board playNext = new Board(board);
            if(!playNext.correctMove(convertIndex(i))) continue;
            playNext.playMove(convertIndex(i));

            int result = minMaxValue(playNext,this.isP1,1);
            if(index==-1){
                maxResult = result;
                index = i;
            }
            if(isP1 && result >maxResult){
                maxResult = result;
                index = i;
            }
            else if(!isP1 && result <maxResult ){
                maxResult = result;
                index = i;
            }
        }
        return convertIndex(index);//Case 1 d'index 0
    }

    public int minMaxValue(Board board,boolean isP1, int depth){
        int sizeArray = board.cells.length/2;
        int[] scoreByIndex = new int[sizeArray];
        if(board.endPosition()){
            if (board.scoreP1 == board.scoreP2) return 0;
            else if (board.scoreP1 > board.scoreP2) return VALMAX;
            else return -VALMAX;
        }
        if(depth==maxDepth){
            return board.scoreP1-board.scoreP2;
        }
        for(int i = 0; i<sizeArray;i++){
            if(board.correctMove(convertIndex(i))){
                Board nextPos = new Board(board);
                nextPos.playMove(convertIndex(i));
                scoreByIndex[i] = minMaxValue(nextPos,!isP1,depth+1);
            }
            else{
                if (isP1) scoreByIndex[i] = -10000;
                else scoreByIndex[i] = +10000;
            }
        }
        int res;
        if(isP1){
            // WRITE the code : res contains the MAX of tab_values
            int max = -1;
            for (int i = 0; i < sizeArray; i++) {
                int score = scoreByIndex[i];
                if (max == -1 || max < score) {
                    max = score;
                }
            }
            res = max;
        }
        else {
            // WRITE the code: res contains the MIN of tab_values
            int min = -1;
            for (int i = 0; i < sizeArray; i++) {
                int score = scoreByIndex[i];
                if (min == -1 || min > score) {
                    min = score;
                }
            }
            res = min;
        }
        return res;

    }

    private int convertIndex(int i){
        if(isP1){
            return i*2;
        }
        else{
            return i*2+1;
        }
    }
}
