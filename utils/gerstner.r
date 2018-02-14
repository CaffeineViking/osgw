# Single Gerstner wave for plotting purposes only.
gerstner_wave <- function(p, t, d, A, Q, w, phi) {
    H <- A*sin((p%*%d)*w + t*phi)
    Gx <- d[1]*Q*A*cos((p%*%d)*w + t*phi)
    Gz <- d[2]*Q*A*cos((p%*%d)*w + t*phi)
    return(c(p[1] + Gx, H, p[2] + Gz))
}
