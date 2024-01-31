source("medidas.R")
ndata <- 3000

normaliza<-function(x){
	resp<-x-min(x)
	resp<-resp/(max(x)-min(x))
	resp
}


tt2 <- seq(0, 9, length=ndata)
senoide <- normaliza(sin(pi * tt2) )

ruido<-rnorm(ndata, mean=0, sd=0.1)

tendencia<-tt2

tseries<-list()

serie1<-senoide
tseries[[1]]<-serie1
serie2<-senoide+ruido
tseries[[2]]<-serie2
serie3<-senoide+ruido+tendencia
tseries[[3]]<-serie3
serie4<-normaliza(cos(pi * tt2))+rnorm(ndata, mean=0, sd=0.1)
tseries[[4]]<-serie4
serie5<-normaliza(cos(pi * tt2))+rnorm(ndata, mean=0, sd=0.1)
tseries[[5]]<-serie5
serie6<-normaliza(cos(pi * tt2))+rnorm(ndata, mean=0, sd=0.1)+tendencia
tseries[[6]]<-serie6


resultadoMetrica <- function(vetorSimilaridades, metrica){	#retorna em ordem as series mais similares
	cat(metrica,": ")
	cat("maior similaridade com as series: [", match(sort(vetorSimilaridades), vetorSimilaridades)[2:length(vetorSimilaridades)], "]\n\n")
}

printEuclidiana <- function(ts){
	cat("##########\n", "Dist. Euclidiana:\n")
	for(i in 1:5){
		temp <- NULL
		for(j in 1:6){
			if(i != j){
				temp <- c(temp, distancia.euclidiana(ts[[i]], ts[[j]]))
				cat("Comparando serie ", i, " com ", j, ":", temp[length(temp)], "\n")
			}
			else
				temp <- c(temp, 0)
		}
		resultadoMetrica(temp,"Dist. Euclidiana")
	}
}

printLBHust <- function(ts, windowSize){
	cat("##########\n LB HUST:\n")
	for(i in 1:5){
		temp <- NULL
		for(j in 1:6){
			if(i != j){
				temp <- c(temp, distancia.lbHust(ts[[i]], ts[[j]], windowSize))
				cat("Comparando serie ", i, " com ", j, ":", temp[length(temp)], "\n")
			}
			else
				temp <- c(temp, 0)
		}
		resultadoMetrica(temp,"LB HUST")
	}
}

printDTW <- function(ts){
	cat("##########\n", "DTW:\n")
	for(i in 1:5){
		temp <- NULL
		for(j in 1:6){
			if(i != j){
				temp <- c(temp, distancia.dtw(ts[[i]], ts[[j]]))
				cat("Comparando serie ", i, " com ", j, ":", temp[length(temp)], "\n")
			}
			else
				temp <- c(temp, 0)
		}
		resultadoMetrica(temp,"DTW")
	}
}

printCrossCorrelation <- function(ts, lag){
	cat("##########\n Cross Correlation:\n")
	for(i in 1:5){
		temp <- NULL
		for(j in 1:6){
			if(i != j){
				temp <- c(temp, distancia.crossCorrelation(ts[[i]], ts[[j]], lag))
				cat("Comparando serie ", i, " com ", j, ":", temp[length(temp)], "\n")
			}
			else
				temp <- c(temp, 0)
		}
		resultadoMetrica(temp,"Cross-correlation")
	}
}

printCID <- function(ts){
	cat("##########\n", "CID:\n")
	for(i in 1:5){
		temp <- NULL
		for(j in 1:6){
			if(i != j){
				temp <- c(temp, distancia.CID(ts[[i]], ts[[j]]))
				cat("Comparando serie ", i, " com ", j, ":", temp[length(temp)], "\n")
			}
			else
				temp <- c(temp, 0)
		}
		resultadoMetrica(temp,"CID")
	}
}

#printEuclidiana(tseries)
#eadline()

#printCID(tseries)
#readline()

#printLBHust(tseries, 200)
#readline()

#printCrossCorrelation(tseries, 0)
#readline()

#printDTW(tseries)

#http://stackoverflow.com/questions/15131926/plot-multiples-time-series-in-r-with-legend
#ts(tseries[[i]], frequency = 4, start = c(1920, 2))


#series <- ts(matrix(rnorm(300), nrow = 100, ncol = 10), start=c(2000, 1), frequency=12)			#so mostra no max 10 TS no plot (ncol = num time series)
#plot.ts(series, main="Exemplo: 10 TS em um gráfico")


#distancia.lbKeogh(tseries[[1]], tseries[[2]], 20)$d
#dtw(tseries[[1]], tseries[[2]], window.type = "slantedband", window.size = 20)$distance
