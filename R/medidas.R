#https://r-how.com/packages/TSdist

call_envelop <- function(tSeries, window.size){
	
	lengthTseries <- length(tSeries)
	wSize <- ceiling(lengthTseries/window.size)
	
	env <- split(tSeries, rep(1:ceiling(wSize), each=window.size)[1:lengthTseries])
	
	envelops <- NULL
	envMin <- rep(0, lengthTseries)
	envMax <- rep(0, lengthTseries)
	
	for(i in 1: lengthTseries){
		envMin[i] <- min(env[[ ceiling(i/wSize) ]])
		envMax[i] <- max(env[[ ceiling(i/wSize) ]])
	}
	
	envelops$min <- envMin
	envelops$max <- envMax
	
	return (envelops)
}

distancia.euclidiana <- function(serieA, serieB){
	sqrt(sum(serieA-serieB)^2)
}

library(dtw)
distancia.dtw <- function(serieA, serieB){
	dtw(serieA, serieB)$distance	#$normalizedDistance
}

distancia.crossCorrelation <- function(serieA, serieB, maxLag){
	#ccf(serieA, serieB)$acf
	#cor(serieA, serieB)
	sA_Length <- length(serieA)
	correlationDist <- 0

	#desloca a serieB para direita
	correlation_lag <- 0
	while(correlation_lag <= maxLag){
		for(i in 1: sA_Length ){
			if(i+correlation_lag > sA_Length)
				break	
		
			correlationDist <- correlationDist + (serieA[i+correlation_lag] * serieB[i])
			#cat("Multiplicando [",  serieA[i+correlation_lag], "*", serieB[i], "] - resultado ate agora: ", correlationDist, "\n")			
		}
		correlation_lag <- correlation_lag + 1		
	}
	
	#desloca a serieB para esquerda
	correlation_lag <- 1
	while(correlation_lag <= maxLag){
		for(i in 1: sA_Length){
			if(i+correlation_lag > sA_Length)
				break
			
			correlationDist <- correlationDist + (serieA[i] * serieB[i+correlation_lag])
		}	
		correlation_lag <- correlation_lag + 1
	}
	
	return(correlationDist)
}

distancia.CID <- function(serieA, serieB){
	sA_Length <- length(serieA)
	sB_Length <- length(serieB)
	
	#complexidade estimada da serie, dada pelo autor:
	complexidadeSerieA <- 0
	complexidadeSerieB <- 0
	
	for(i in 1: (sA_Length-1)){
		complexidadeSerieA <- complexidadeSerieA + (serieA[i] - serieA[i+1])^2
	}
	complexidadeSerieA <- sqrt(complexidadeSerieA)
	
	for(i in 1: (sB_Length-1)){
		complexidadeSerieB <- complexidadeSerieB + (serieB[i] - serieB[i+1])^2
	}
	complexidadeSerieB <- sqrt(complexidadeSerieB)
	
	#complexity correction factor (CF)
	CF <- max(complexidadeSerieA, complexidadeSerieB) / min(complexidadeSerieA, complexidadeSerieB)
	
	return (distancia.euclidiana(serieA, serieB) * CF)

}

distancia.lbKeogh <- function(x, y, window.size = NULL, norm = "L1",
                     lower.env = NULL, upper.env = NULL, force.symmetry = FALSE) {
	
	#https://github.com/asardaes/dtwclust/blob/master/R/lb-keogh.R#L101
	
     norm <- match.arg(norm, c("L1", "L2"))

     #consistency_check(x, "ts")

     if (is.null(lower.env) || is.null(upper.env)) {
          #consistency_check(y, "ts")

          if (length(x) != length(y))
               stop("The series must have the same length")

          #window.size <- consistency_check(window.size, "window")

          if (window.size > length(x))
               stop("The width of the window should not exceed the length of the series")
     }

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

     if (length(lower.env) != length(x))
          stop("Length mismatch between 'x' and the lower envelope")

     if (length(upper.env) != length(x))
          stop("Length mismatch between 'x' and the upper envelope")

     D <- rep(0, length(x))

     ind1 <- x > upper.env
     D[ind1] <- x[ind1] - upper.env[ind1]
     ind2 <- x < lower.env
     D[ind2] <- lower.env[ind2] - x[ind2]

     d <- switch(EXPR = norm,
                 L1 = sum(D),
                 L2 = sqrt(sum(D^2)))

     if (force.symmetry) {
          d2 <- lb_keogh(x = y, y = x, window.size = window.size, norm = norm)

          if (d2$d > d) {
               d <- d2$d
               lower.env = d2$lower.env
               upper.env = d2$upper.env
          }
     }

     ## Finish
     list(d = d,
          upper.env = upper.env,
          lower.env = lower.env)
}

distancia.lbHust <- function(serieA, serieB, window.size){
	distancia <- 0
	
	envelopes <- call_envelop(y, window.size*2L + 1L)
	lower.env <- envelopes$min
	upper.env <- envelopes$max
	
	#series devem ter o mesmo tamanho!
	
	for(i in 1: sA_Length){
		#pegando a subsequencia da serie (janela), os elementos [i-window.size : i+window.size+1], ie, 2w+1
		UpperBound_serieA <- max( serieA[ min( i - window.size > 0) : max(i + window.size +1 < sA_Length) ] )
		LowerBound_serieA <- min( serieA[ min( i - window.size > 0) : max(i + window.size +1 < sA_Length) ] )
		
		UpperBound_serieB <- max( serieB[ min( i - window.size > 0) : max(i + window.size +1 < sB_Length) ] )
		LowerBound_serieB <- min( serieB[ min( i - window.size > 0) : max(i + window.size +1 < sB_Length) ] )
		
		if(LowerBound_serieA > UpperBound_serieB)
			distancia <- distancia + (LowerBound_serieA - UpperBound_serieB)^2
		else if(LowerBound_serieB > UpperBound_serieA)
			distancia <- distancia + (LowerBound_serieB - UpperBound_serieA)^2
		#0, caso contrario
	}
	
	return(distancia)
}