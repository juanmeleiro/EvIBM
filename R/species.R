setwd("./species")
file.names <- dir()

number.spp=data.frame()

for (i in 1:(length(file.names)-1)){
  dados <- read.csv(paste(file.names[i]), head=TRUE, sep=";")
  number.spp <- rbind(number.spp, dados)
}
setwd("../")

colnames(number.spp) <- c("gen","sp", "sim")

# Read data
nuber.spp <- data.matrix(number.spp)

# Define limits for the graph
max.spp <- max(number.spp[,2])
max.time <- max(number.spp[,1])

number.data <- aggregate( . ~ gen, FUN=function(x) c(mn=mean(x), sd=sd(x)), data=number.spp)
sumario <- do.call (data.frame, number.data)

number.fig <-
  ggplot() +  
  geom_point(data = subset(number.spp), aes(x = gen, y = sp), size=0.9, color=color.sp[1], alpha=0.3)+
  geom_line(data = subset(sumario), aes(x = gen, y = sp.mn), size=0.5, color=color.sp[2])+
  geom_ribbon(data = subset(sumario), aes(x = gen, ymin=sp.mn-sp.sd, ymax=sp.mn+sp.sd),alpha=0.3, color = color.sp[3], fill = color.sp[4])+
  guides(fill=FALSE, shape="none") +
  labs(x = "geração", y = "Número de espécies") +  
  xlim(0, max.time) +
  ylim(0, max.spp) +
  theme_bw()+
  theme(text = element_text(size=12, family="Helvetica"),
        panel.grid.minor = element_blank(),
        panel.grid.major = element_blank(), 
        legend.title=element_blank(),
        legend.position = c(1.12,0.365), 
        #legend.background = element_rect(fill="transparent",size=0.01, linetype="solid",colour ="orange"),
        #legend.key = element_rect(fill = "transparent", colour = "transparent"),
        legend.text=element_text(size=5),
        legend.box.background = element_rect(),
        legend.margin = margin(-4, 4, -1, -1),
        plot.margin = unit(c(0.1,2,0.1,0.1), "cm"))

#gsave("./figs/base/nspp.png")
