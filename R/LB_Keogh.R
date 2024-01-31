# https://github.com/asardaes/dtwclust/blob/master/R/lb-keogh.R

lb_keogh <- function(x, y, window.size = NULL, norm = "L1", lower.env = NULL, upper.env = NULL) {

     norm <- match.arg(norm, c("L1", "L2"))

     consistency_check(x, "ts")
     consistency_check(y, "ts")

     if (length(x) != length(y))
          stop("The series must have the same length")

     window.size <- consistency_check(window.size, "window")

     if (window.size > length(x))
          stop("The width of the window should not exceed the length of the series")

     ## NOTE: the 'window.size' definition varies betwen 'dtw' and 'runminmax'
     if (is.null(lower.env) && is.null(upper.env)) {
          envelopes <- call_envelop(y, window.size*2L + 1L)
          lower.env <- envelopes$min
          upper.env <- envelopes$max

     } else if (is.null(lower.env)) {
          lower.env <- caTools::runmin(y, window.size*2L + 1L)

     } else if (is.null(upper.env)) {
          upper.env <- caTools::runmax(y, window.size*2L + 1L)
     }

     if (length(lower.env) != length(y))
          stop("Length mismatch between 'y' and its lower envelope")

     if (length(upper.env) != length(y))
          stop("Length mismatch between 'y' and its upper envelope")

     D <- rep(0, length(x))

     ind1 <- x > upper.env
     D[ind1] <- x[ind1] - upper.env[ind1]
     ind2 <- x < lower.env
     D[ind2] <- lower.env[ind2] - x[ind2]

     d <- switch(EXPR = norm,
                 L1 = sum(D),
                 L2 = sqrt(sum(D^2)))

     list(d = d,
          upper.env = upper.env,
          lower.env = lower.env)
}


############

http://stackoverflow.com/questions/18857275/r-given-a-list-return-a-list-of-equal-length-sublists

http://stackoverflow.com/questions/31561238/lapply-function-loops-on-list-of-lists-r

http://stackoverflow.com/questions/5629069/iterating-over-separate-lists-in-r

#LB_Keogh(S, T) = SUM(i:n) { (Si - Uti)², Si > Uti;		 (Si - Lti)², Si < Lti; 		 0, caso contrario }

#LB_Hust(S, T) = SUM(i:n)  { (Lsi - Uti)², Lsi > Uti;	 (Lti - Usi)², Lti > Usi; 		 0, caso contrario }

lb_hust <- function(xTs, yTs, window.size){

	#obs.: xTs e yTs devem ser do mesmo tamanho
	xLength <- length(xTs)
	yLength <- length(yTs)
	
	xEnvelop <- split(xTs, rep(1:ceiling(xLength/window.size), each=window.size)[1:xLength])
	yEnvelop <- split(yTs, rep(1:ceiling(yLength/window.size), each=window.size)[1:yLength])
	
	#Lower e Upper Bounds da serie xTs
	xLowerBounds <- NULL
	xUpperBounds <- NULL
	
	#Lower e Upper Bounds da serie yTs
	yLowerBounds <- NULL
	yUpperBounds <- NULL
	
	distance <- 0
	
	for(i in 1:length(xEnvelop)){
		xLowerBounds <- c(xLowerBounds, lapply(xEnvelop[1], min)[[1]] )
		xUpperBounds <- c(xUpperBounds, lapply(xEnvelop[1], max)[[1]]  )
		
		yLowerBounds <- c(yLowerBounds, lapply(yEnvelop[1], min)[[1]]  )
		yUpperBounds <- c(yUpperBounds, lapply(yEnvelop[1], max)[[1]]  )
		
		if(xLowerBounds[i] > yUpperBounds[i])
			distance <- distance + (xLowerBounds[i] - yUpperBounds[i])^2
		else if(yLowerBounds[i] > xUpperBounds[i])
			distance <- distance + (yLowerBounds[i] - xUpperBounds[i])^2
		#0, caso contrario
	}
	
	return(distance)
}

##################
#Testar:
sequencia <- seq(from = 0, to = 2*pi, len = 100)
serie1 <- sin(sequencia) + runif(100)/10
serie2 <- cos(sequencia) + runif(100)/10
lb_hust(serie1, serie2, 10)

####################