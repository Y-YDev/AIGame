package program;

public class AI3 implements IPlayer {

    private boolean isP1 = false;
    public final static int VALMAX=96;
    public int maxDepth;
    
    private Pool pool;
    

    public AI3(boolean isP1,int maxDepth){
        this.isP1 = isP1;
        this.maxDepth = maxDepth;
        pool = new Pool(maxDepth+5,maxDepth+5);
    }


    @Override
    public int play(Board board){
        int maxResult= 0;
        int index = -1;
        for(int i = 0; i<board.currentSize/2;i++){
            Board playNext = pool.pollBoard(board);
            
            if(!playNext.correctMove(convertIndex(i))) {
            	 pool.pushBoard(playNext);
            	continue;
            }
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
            pool.pushBoard(playNext);
        }
        return convertIndex(index);//Case 1 d'index 0
    }

    public int minMaxValue(Board board,boolean isP1, int depth){
        int sizeArray = board.currentSize/2;

        if(board.endPosition()){
            if (board.scoreP1 == board.scoreP2) return 0;
            else if (board.scoreP1 > board.scoreP2) return VALMAX;
            else return -VALMAX;
        }
        if(depth==maxDepth){
            return evaluation(board);
        }
        
        int[] scoreByIndex = pool.pollIntArray12();
        for(int i = 0; i<sizeArray;i++){
            if(board.correctMove(convertIndex(i))){
                Board nextPos = pool.pollBoard(board);
                nextPos.playMove(convertIndex(i));
                scoreByIndex[i] = minMaxValue(nextPos,!isP1,depth+1);
                pool.pushBoard(nextPos);
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
        pool.pushIntArray12(scoreByIndex);
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
    
    
    private int evaluation(Board board) {

    	if(board.scoreP1>48) return 10000;
    	if(board.scoreP2>48) return -10000;
    	return board.scoreP1-board.scoreP2;
    	
    }
}
