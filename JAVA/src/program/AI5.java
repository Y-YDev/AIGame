package program;

import static program.Board.CHEATVALUE;
import static program.Board.MAXVALUE;

public class AI5 implements IPlayer {

    private boolean isP1;
    public final static int VALMAX=96;
    private int winValue = 10000000;

    private int minDepth;
    private int minDepthReduce;
    private int capeMax = 22;
    public int maxDepth;
    private int turn = 0;
    private Pool pool;
    private long time=500;


    public AI5(boolean isP1,int maxDepth){
        this.isP1 = isP1;
        this.maxDepth = maxDepth;
        minDepth = maxDepth;
        minDepthReduce = 15;

        pool = new Pool(maxDepth+101,0);
    }


    @Override
    public int play(Board board){
        long start = System.currentTimeMillis();
        turn++;
        if(turn==1){
            if(isP1) return convertIndex(isP1,1);
            return convertIndex(isP1,6);
        }
        if(time<200){
            maxDepth++;
        }
        if(time<10){
            maxDepth++;
        }
        else if(time>1700 && maxDepth>minDepth){
            maxDepth--;
        }
        if(capeMax<maxDepth){
            maxDepth = capeMax;
        }
        if(board.isReduce&& maxDepth<minDepthReduce){
            maxDepth = minDepthReduce;
        }
        System.out.println("depth : "+maxDepth);
        int maxResult= 0;
        int index = -1;
        int alpha = Integer.MIN_VALUE;
        int beta = Integer.MAX_VALUE;

        for(int i = 0; i<board.currentSize/2;i++){
            Board playNext = pool.pollBoard(board);

            if(!playNext.correctMove(convertIndex(isP1,i))) {
                pool.pushBoard(playNext);
                continue;
            }
            //l'ia fait tout les coup possible 1 par 1
            playNext.playMove(convertIndex(isP1,i));
            //l'ia fait jouer les autre coup en partant de l'inverse
            int result = minMaxValue(playNext,!this.isP1,1,alpha,beta);

            //On retient le coup le plus avantageux
            if(index==-1){
                maxResult = result;
                index = i;
            }
            if(isP1 && result >maxResult){
                maxResult = result;
                index = i;
                alpha =result;

            }
            else if(!isP1 && result <maxResult ){
                maxResult = result;
                index = i;
                beta =result;

            }
            pool.pushBoard(playNext);
        }
        long end = System.currentTimeMillis();
        time = end-start;
        System.out.println(((double)time)/1000);
        System.out.println("max result = "+maxResult);
        System.out.println("----------------------------------");
        return convertIndex(isP1, index);//Case 1 d'index 0
    }

    public int minMaxValue(Board board,boolean rushMax, int depth,int alpha, int beta){
        int sizeArray = board.currentSize/2;

        if(board.endPosition()){
            if (board.scoreP1 == board.scoreP2) return 0;
            else if (board.scoreP1 > board.scoreP2) return winValue+ board.scoreP1- board.scoreP2;
            else return -winValue+board.scoreP1- board.scoreP2;
        }
        if(depth==maxDepth){
            return evaluation(board);
        }

        int currentScore;
        for(int i = 0; i<sizeArray;i++){
            if(board.correctMove(convertIndex(rushMax,i))){
                Board nextPos = pool.pollBoard(board);
                nextPos.playMove(convertIndex(rushMax,i));
                currentScore = minMaxValue(nextPos,!rushMax,depth+1,alpha,beta);
                pool.pushBoard(nextPos);

                if(rushMax){
                    if(alpha<=currentScore){
                        alpha =currentScore;
                        if (alpha>=beta){
                            return beta;
                        }
                    }
                }
                else {
                    if (beta >= currentScore) {
                        beta = currentScore;
                        if (beta <= alpha) {
                            return alpha;
                        }
                    }
                }
            }
        }

        if(rushMax) return alpha;
        return beta;

    }

    private int convertIndex(boolean isP1, int i){
        if(isP1){
            return i*2;
        }
        else{
            return i*2+1;
        }
    }
    private int evaluation4(Board board) {

        if(board.scoreP1>48) return winValue+board.scoreP1-board.scoreP2;
        if(board.scoreP2>48) return -winValue+board.scoreP1-board.scoreP2;
        int score = (board.scoreP1-  board.scoreP2)*1000;
        int max = -1;
        if(board.isReduce){
            for (int i = 0; i < board.currentSize; i+=2) {
                if(board.cells[i]>max) {
                    max = board.cells[i];
                }
            }
            score+=max;
            max = -1;
            for (int i = 1; i < board.currentSize; i+=2) {
                if(board.cells[i]>max) {
                    max = board.cells[i];
                }
            }}
        else{
            for (int i = 0; i < board.currentSize; i+=4) {
                if(board.cells[i]>max) {
                    max = board.cells[i];
                }
            }
            score+=max;
            max = -1;
            for (int i = 2; i < board.currentSize; i+=4) {
                if(board.cells[i]>max) {
                    max = board.cells[i];
                }
            }}
        score+=max;




        return score;


    }

    private int evaluation2(Board board) {

        if(board.scoreP1>48) return winValue+board.scoreP1-board.scoreP2;
        if(board.scoreP2>48) return -winValue+board.scoreP1-board.scoreP2;
        int score = (board.scoreP1-  board.scoreP2)*1000;
//        for (int i = 0; i < board.currentSize; i+=2) {
//
//            score+=board.cells[i];
//        }
//        for (int i = 1; i < board.currentSize; i+=2) {
//            score-=board.cells[i];
//        }

        return score;
    }

    private int evaluation(Board board) {

        if(board.scoreP1>48) return winValue+board.scoreP1-board.scoreP2;
        if(board.scoreP2>48) return -winValue+board.scoreP1-board.scoreP2;
        int score = (board.scoreP1-  board.scoreP2)*1000;

        for (int i = 0; i < board.currentSize; i++) {
            if(i%4<2)
                score+=board.cells[i];
            else
                score-=board.cells[i];
        }


        return score;
    }


    private int evaluation3(Board board) {

        if(board.scoreP1>48) return winValue+board.scoreP1-board.scoreP2;
        if(board.scoreP2>48) return -winValue+board.scoreP1-board.scoreP2;
        int score = (board.scoreP1-  board.scoreP2)*1000;
        int max=-1;
        for (int i = 0; i < board.currentSize; i+=2) {
            if(board.cells[i]>max) {
                max = board.cells[i];
            }
        }
        score+=max;
        max = -1;
        for (int i = 1; i < board.currentSize; i+=2) {
            if(board.cells[i]>max) {
                max = board.cells[i];
            }
        }

        return score;
    }
}
