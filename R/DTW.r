library(dtw)

sequencia <- seq(from = 0, to = 2*pi, len = 100)
serie1 <- sin(sequencia) + runif(100)/10
serie2 <- cos(sequencia) + runif(100)/10
alinhamento <- dtw(serie1, serie2, step = asymmetricP1, keep = T, window.type = "itakura")		#window.type = "none", "itakura", "sakoechiba", "slantedband";  #steps: symmetric1, symmetric2 , asymmetric, ...

plot(alinhamento, type="two", xlab = c("Distancia entre series: ", alinhamento$distance) )		#mostrar alinhamento entre as duas series


# Clustering TS:


series <- ts(matrix(rnorm(300), nrow = 100, ncol = 10), start=c(2000, 1), frequency=12)			#so mostra no max 10 TS no plot (ncol = num time series)
plot.ts(series, main="Exemplo: 10 TS")

distMatrix <- dist(series, method="DTW")
hc <- hclust(distMatrix, method="average")														#method = "ward.D", "ward.D2", "single", "complete", "average" (= UPGMA), "mcquitty" (= WPGMA), "median" (= WPGMC) or "centroid" (= UPGMC)

plot(hc, main="Exemplo: Clustering 10 TS")