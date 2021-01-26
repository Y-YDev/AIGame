package program;

public class AI4 implements IPlayer {

    private boolean isP1 = false;
    public final static int VALMAX=96;
    public int maxDepth;

    private Pool pool;


    public AI4(boolean isP1,int maxDepth){
        this.isP1 = isP1;
        this.maxDepth = maxDepth;
        pool = new Pool(maxDepth+5,maxDepth+5);
    }


    @Override
    public int play(Board board){
        int maxResult= 0;
        int index = -1;
        int alpha = -10000;
        int beta = 10000;
        for(int i = 0; i<board.currentSize/2;i++){
            Board playNext = pool.pollBoard(board);

            if(!playNext.correctMove(convertIndex(i))) {
                pool.pushBoard(playNext);
                continue;
            }
            playNext.playMove(convertIndex(i));

            int result = minMaxValue(playNext,this.isP1,1,alpha,beta);
            if(index==-1){
                maxResult = result;
                index = i;
            }
            if(isP1 && result >=maxResult){
                maxResult = result;
                index = i;
                alpha =result;
                if (alpha>=beta){
                    return alpha;
                }
            }
            else if(!isP1 && result <=maxResult ){
                maxResult = result;
                index = i;
                beta =result;
                if (alpha>=beta){
                    return beta;
                }
            }
            pool.pushBoard(playNext);
        }
        return convertIndex(index);//Case 1 d'index 0
    }

    public int minMaxValue(Board board,boolean isP1, int depth, int alpha, int beta){
        int sizeArray = board.currentSize/2;

        if(board.endPosition()){
            if (board.scoreP1 == board.scoreP2) return 0;
            else if (board.scoreP1 > board.scoreP2) return VALMAX;
            else return -VALMAX;
        }
        if(depth==maxDepth){
            return evaluation(board);
        }

        int currentScore;

        for(int i = 0; i<sizeArray;i++){
            if(board.correctMove(convertIndex(i))){
                Board nextPos = pool.pollBoard(board);
                nextPos.playMove(convertIndex(i));
                currentScore = minMaxValue(nextPos,!isP1,depth+1,alpha,beta);
                pool.pushBoard(nextPos);
                if(isP1){
                    if(alpha>=currentScore){
                        alpha =currentScore;
                        if (alpha>=beta){
                            return alpha;
                        }
                    }
                }
                else {
                    if (beta <= currentScore) {
                        beta = currentScore;
                        if (beta <= alpha) {
                            return beta;
                        }
                    }
                }
            }
        }
        if(isP1) return alpha;
        return beta;

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
